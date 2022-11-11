#include "MoveGeneratorV2.hpp"

#include <numeric>

#include <detail/CheckChecker.hpp>
#include <detail/PinnedMovesChecker.hpp>

namespace MoveGenerator
{


MoveGeneratorV2::MoveGeneratorV2(const Board& board, NOTATION::COLOR::color playerToGenerate)
	: board(board)
	, pieceColor(playerToGenerate)
	, state(State::Uninitialized)
{
}

unsigned MoveGeneratorV2::getValidMoveCount(NotCheckedTag)
{
	if (state == State::Uninitialized)
	{
		kingPosition = CheckChecker::findKing(board, pieceColor);
		kingAttackersCount = 0;
		calculateMoveTables();
		state = State::MoveTablesCalculated;
		calculateMoveCount();
	}
	return moveCount;
}

unsigned MoveGeneratorV2::getValidMoveCount()
{
	if (state == State::Uninitialized)
	{
		kingPosition = CheckChecker::findKing(board, pieceColor);
		auto out = CheckChecker::isCheckedBeforeMoveExtendeded(board, pieceColor, kingPosition);
		kingAttackersCount = out.attackersNum;
		possibleBlockersMask = out.possibleBlockersMask;
		calculateMoveTables();
		state = State::MoveTablesCalculated;
		calculateMoveCount();
	}
	return moveCount;
}

void MoveGeneratorV2::evaluateKnights(uint64_t knightsBitMask, uint64_t forbidenFields)
{
    while (knightsBitMask)
    {
        unsigned char knightIndex = 63 - __builtin_clzll(knightsBitMask);
        auto knightBitMask = 1ull << knightIndex;

        auto moveTable = bitBoardLookup[knightIndex].knightsMovePossibilities & ~forbidenFields;
        moveTables.push_back(MoveTable{MoveTable::Type::KnightMoves, knightIndex, moveTable});

        knightsBitMask ^= knightBitMask;
    }
};

void MoveGeneratorV2::evaluateKing(uint64_t forbidenFields)
{
    auto pseudoPossibleMovesTable = bitBoardLookup[kingPosition].kingMovePossibilities & ~forbidenFields;
    uint64_t possibleMovesTable = 0ull;

    // check if king is checked after king move
    {
	    board.piecesBitSets[(int)pieceColor].kingsMask ^= (1ull << kingPosition);
	    while (pseudoPossibleMovesTable)
	    {
	    	unsigned char kingDestinationIndex = 63 - __builtin_clzll(pseudoPossibleMovesTable);
	        auto kingDestinationBitMask = 1ull << kingDestinationIndex;

	        if (not CheckChecker::isAttackedOn(board, pieceColor, kingDestinationIndex))
	        {
	        	possibleMovesTable |= kingDestinationBitMask;
	        }

	        pseudoPossibleMovesTable ^= kingDestinationBitMask;
	    }
		board.piecesBitSets[(int)pieceColor].kingsMask ^= (1ull << kingPosition);
	}
    moveTables.push_back(MoveTable{MoveTable::Type::KingMoves, kingPosition, possibleMovesTable});
}

/*
	In UTs it shall be checked:
		- non beating
			- whether we can move pawns on sigle fields
			- we can move pawns two fields
			- we move pined pawns only if no check is caused then.
			- in the situation king is checked by one attacker, we can move pawn to block it.
		- beating (To be analyzed yet)

*/
void MoveGeneratorV2::evaluatePawns(uint64_t pawnsBitMask, uint64_t forbidenFields)
{
	constexpr uint64_t LINE_4 = 0x00'00'00'00'FF'00'00'00ull;
    constexpr uint64_t LINE_5 = 0x00'00'00'FF'00'00'00'00ull;

	constexpr unsigned char NOT_RELEVANT = 0u;
	if (pieceColor == NOTATION::COLOR::color::white)
	{
		auto singleMovedPawns = pawnsBitMask <<= 8;
		auto moveTable = singleMovedPawns & (~forbidenFields);
		moveTables.push_back(MoveTable{MoveTable::Type::SinglePawnMoves, NOT_RELEVANT, moveTable});

		auto doubleMovedPawns = (moveTable <<= 8) & LINE_4;
		moveTable = doubleMovedPawns & (~forbidenFields);
		moveTables.push_back(MoveTable{MoveTable::Type::DoublePawnMoves, NOT_RELEVANT, moveTable});
	}
	else
	{
		auto singleMovedPawns = pawnsBitMask >>= 8;
		auto moveTable = singleMovedPawns & (~forbidenFields);
		moveTables.push_back(MoveTable{MoveTable::Type::SinglePawnMoves, NOT_RELEVANT, moveTable});

		auto doubleMovedPawns = (moveTable >>= 8) & LINE_5;
		moveTable = doubleMovedPawns & (~forbidenFields);
		moveTables.push_back(MoveTable{MoveTable::Type::DoublePawnMoves, NOT_RELEVANT, moveTable});
	}
}

void MoveGeneratorV2::calculateMoveTables()
{
    const auto pinnedFields = findPinned(board, pieceColor, kingPosition);
    const auto ownFields = getAllOccupiedPerColor(board, pieceColor);
    const auto oponentFields = getAllOccupiedPerColor(board, pieceColor+1);
    const auto allFields = ownFields | oponentFields;

    if (kingAttackersCount)
    {
    	if (kingAttackersCount == 1)
    	{
    		const auto forbidenFields = ownFields | (~possibleBlockersMask);
    		const auto notPinnedKnights = board.piecesBitSets[static_cast<unsigned char>(pieceColor)].knightsMask & (~pinnedFields.allPinned);
        	evaluateKnights(notPinnedKnights, forbidenFields);
        	evaluateKing(ownFields);  // tmp
        	return;
    	}
    	else
    	{
    		evaluateKing(0ull);  // tmp
    		return;
    	}
    }
    else
    {
    	const auto notPinnedKnights = board.piecesBitSets[static_cast<unsigned char>(pieceColor)].knightsMask & (~pinnedFields.allPinned);
        evaluateKnights(notPinnedKnights, ownFields);
        evaluateKing(ownFields);  // tmp
        const auto pawns = board.piecesBitSets[static_cast<unsigned char>(pieceColor)].pawnsMask & (~(pinnedFields.allPinned ^ pinnedFields.verticallyPinned)); // tmp. We can move vertically pined figures
        evaluatePawns(pawns, allFields);
        return;
    }
}

void MoveGeneratorV2::calculateMoveCount()
{
	moveCount = 0;
	for (auto&& mt: moveTables)
	{
		moveCount += __builtin_popcountll(mt.bitField);
	}
}

}  // namespace MoveGenerator