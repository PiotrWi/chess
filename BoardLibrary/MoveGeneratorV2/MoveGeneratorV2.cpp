#include "MoveGeneratorV2.hpp"

#include <numeric>

#include "PositionAnalyzers/CheckChecker.hpp"
#include "PositionAnalyzers/PinnedMovesChecker.hpp"

namespace MoveGenerator
{


MoveGeneratorV2::MoveGeneratorV2(const Board& board, const NOTATION::COLOR::color playerToGenerate)
	: board(board)
	, pieceColor(playerToGenerate)
{
}

unsigned MoveGeneratorV2::getValidMoveCount(NotCheckedTag)
{
    kingPosition = CheckChecker::findKing(board, pieceColor);
    kingAttackersCount = 0;
    possibleBlockersMask = 0;
    fieldsSeenByOpponent = getFieldsSeenByOpponent(board, pieceColor, kingPosition);
    calculateMoveTables();
    calculateMoveCount();
    return moveCount;
}

unsigned MoveGeneratorV2::getValidMoveCount()
{
	kingPosition = CheckChecker::findKing(board, pieceColor);
	auto out = analyzePosition(board, pieceColor, kingPosition);
	kingAttackersCount = out.kingAttackersNum;
	possibleBlockersMask = out.possibleBlockersMask;
    fieldsSeenByOpponent = out.fieldsSeenByOpponent;
	calculateMoveTables();
	calculateMoveCount();
	return moveCount;
}

void MoveGeneratorV2::evaluateKnights(uint64_t knightsBitMask, const uint64_t forbidenFields)
{
    for (; knightsBitMask; knightsBitMask = _blsr_u64(knightsBitMask))
    {
        unsigned char knightIndex = _tzcnt_u64(knightsBitMask);
        auto moveTable = bitBoardLookup[knightIndex].knightsMovePossibilities & ~forbidenFields;
        moveTables[moveTablesN++] = MoveTable{MoveTable::Type::KnightMoves, knightIndex, moveTable};
    }
};

void MoveGeneratorV2::evaluateKing(const uint64_t forbidenFields)
{
    auto pseudoPossibleMovesTable = bitBoardLookup[kingPosition].kingMovePossibilities & ~forbidenFields;

    if (pseudoPossibleMovesTable) moveTables[moveTablesN++] = MoveTable{MoveTable::Type::KingMoves, kingPosition, pseudoPossibleMovesTable};
}

void MoveGeneratorV2::evaluatePawns(uint64_t pawnsBitMask, const uint64_t opponentPieces)
{
	constexpr uint64_t LINE_4 = 0x00'00'00'00'FF'00'00'00ull;
    constexpr uint64_t LINE_5 = 0x00'00'00'FF'00'00'00'00ull;

	constexpr unsigned char NOT_RELEVANT = 0u;
	if (pieceColor == NOTATION::COLOR::color::white)
	{
		auto singleMovedPawns = pawnsBitMask <<= 8;
		auto moveTable = singleMovedPawns & (~opponentPieces);
		moveTables[moveTablesN++] = MoveTable{MoveTable::Type::SinglePawnMoves, NOT_RELEVANT, moveTable};

		auto doubleMovedPawns = (moveTable <<= 8) & LINE_4;
		moveTable = doubleMovedPawns & (~opponentPieces);
		moveTables[moveTablesN++] = MoveTable{MoveTable::Type::DoublePawnMoves, NOT_RELEVANT, moveTable};
	}
	else
	{
		auto singleMovedPawns = pawnsBitMask >>= 8;
		auto moveTable = singleMovedPawns & (~opponentPieces);
		moveTables[moveTablesN++] = MoveTable{MoveTable::Type::SinglePawnMoves, NOT_RELEVANT, moveTable};

		auto doubleMovedPawns = (moveTable >>= 8) & LINE_5;
		moveTable = doubleMovedPawns & (~opponentPieces);
		moveTables[moveTablesN++] = MoveTable{MoveTable::Type::DoublePawnMoves, NOT_RELEVANT, moveTable};
	}
}

void MoveGeneratorV2::evaluatePawns(uint64_t pawnsBitMask, const uint64_t opponentPieces, const uint64_t checkBlockers)
{
	constexpr uint64_t LINE_4 = 0x00'00'00'00'FF'00'00'00ull;
    constexpr uint64_t LINE_5 = 0x00'00'00'FF'00'00'00'00ull;

	constexpr unsigned char NOT_RELEVANT = 0u;
	if (pieceColor == NOTATION::COLOR::color::white)
	{
		auto singleMovedPawns = (pawnsBitMask <<= 8) & (~opponentPieces);
		auto moveTable = singleMovedPawns & checkBlockers;
		moveTables[moveTablesN++] = MoveTable{MoveTable::Type::SinglePawnMoves, NOT_RELEVANT, moveTable};

		auto doubleMovedPawns = (singleMovedPawns <<= 8) & LINE_4;
		moveTable = doubleMovedPawns & (~opponentPieces) & checkBlockers;
		moveTables[moveTablesN++] = MoveTable{MoveTable::Type::DoublePawnMoves, NOT_RELEVANT, moveTable};
	}
	else
	{
		auto singleMovedPawns = (pawnsBitMask >>= 8) & (~opponentPieces);
		auto moveTable = singleMovedPawns & checkBlockers;
		moveTables[moveTablesN++] = MoveTable{MoveTable::Type::SinglePawnMoves, NOT_RELEVANT, moveTable};

		auto doubleMovedPawns = (singleMovedPawns >>= 8) & LINE_5;
		moveTable = doubleMovedPawns & (~opponentPieces) & checkBlockers;
		moveTables[moveTablesN++] = MoveTable{MoveTable::Type::DoublePawnMoves, NOT_RELEVANT, moveTable};
	}
}

void MoveGeneratorV2::evaluatePawnsBeatings(uint64_t pawnsMovingOnRightTopDiagonal, uint64_t pawnsToMoveToLeftTop, uint64_t opponentPieces)
{
	constexpr unsigned char NOT_RELEVANT = 0u;

    if (pieceColor == NOTATION::COLOR::color::white)
	{
		auto beatingsCandidates = ((NOT_A_COL & pawnsToMoveToLeftTop) << 7);
		auto moveTable = beatingsCandidates & opponentPieces;
		moveTables[moveTablesN++] = MoveTable{MoveTable::Type::PawnBeatingsToLeft, NOT_RELEVANT, moveTable};
		
		beatingsCandidates = ((NOT_H_COL & pawnsMovingOnRightTopDiagonal) << 9);
		moveTable = beatingsCandidates & opponentPieces;
		moveTables[moveTablesN++] = MoveTable{MoveTable::Type::PawnBeatingsToRight, NOT_RELEVANT, moveTable};
	}
	else
	{
		auto beatingsCandidates = ((NOT_A_COL & pawnsMovingOnRightTopDiagonal) >> 9);
		auto moveTable = beatingsCandidates & opponentPieces;
		moveTables[moveTablesN++] = MoveTable{MoveTable::Type::PawnBeatingsToLeft, NOT_RELEVANT, moveTable};
		
		beatingsCandidates = ((NOT_H_COL & pawnsToMoveToLeftTop) >> 7);
		moveTable = beatingsCandidates & opponentPieces;
		moveTables[moveTablesN++] = MoveTable{MoveTable::Type::PawnBeatingsToRight, NOT_RELEVANT, moveTable};
	}
}

void MoveGeneratorV2::evaluateEnPassant(uint64_t pawnsMovingOnRightTopDiagonal, uint64_t pawnsToMoveToLeftTop)
{
	if (board.validEnPassant == -1)
	{
		return;
	}
	uint64_t validEnPassantMask = 1ull << board.validEnPassant;
	constexpr unsigned char NOT_RELEVANT = 0u;

	auto checkIfUnderCheckAfterEnPassant = [](auto color, auto boardCopy, auto whitePawn, auto blackPawn, auto validEnPassantMask)
	{
		boardCopy.piecesBitSets[NOTATION::COLOR::WHITE].pawnsMask ^= whitePawn;
		boardCopy.piecesBitSets[NOTATION::COLOR::BLACK].pawnsMask ^= blackPawn;
        if (color == NOTATION::COLOR::WHITE)
        {
            boardCopy.piecesBitSets[NOTATION::COLOR::BLACK].pawnsMask ^= validEnPassantMask;
        }
        else
        {
            boardCopy.piecesBitSets[NOTATION::COLOR::WHITE].pawnsMask ^= validEnPassantMask;
        }
		return CheckChecker::isCheckOn(boardCopy, color);
	};

    if (pieceColor == NOTATION::COLOR::color::white)
	{
		// to left beatings
		auto beatingsCandidates = ((NOT_A_COL & pawnsToMoveToLeftTop) << 7);
		auto moveTable = beatingsCandidates & validEnPassantMask;
		if (moveTable)
		{
			auto whiteBeatingPawnLocation = moveTable >> 7;
			auto blackPawnLocation = validEnPassantMask >> 8;

			if (not checkIfUnderCheckAfterEnPassant(pieceColor, board, whiteBeatingPawnLocation, blackPawnLocation, validEnPassantMask))
			{
				moveTables[moveTablesN++] = MoveTable{MoveTable::Type::PawnBeatingsToLeft, NOT_RELEVANT, moveTable};
			}
		}
		
		beatingsCandidates = ((NOT_H_COL & pawnsMovingOnRightTopDiagonal) << 9);
		moveTable = beatingsCandidates & validEnPassantMask;
		if (moveTable)
		{
			auto whiteBeatingPawnLocation = moveTable >> 9;
			auto blackPawnLocation = validEnPassantMask >> 8;
			if (not checkIfUnderCheckAfterEnPassant(pieceColor, board, whiteBeatingPawnLocation, blackPawnLocation, validEnPassantMask))
			{
				moveTables[moveTablesN++] = MoveTable{MoveTable::Type::PawnBeatingsToRight, NOT_RELEVANT, moveTable};
			}
		}
	}
	else
	{
		// to left beatings
		auto beatingsCandidates = ((NOT_A_COL & pawnsMovingOnRightTopDiagonal) >> 9);
		auto moveTable = beatingsCandidates & validEnPassantMask;
		if (moveTable)
		{
			auto blackBeatingPawnLocation = moveTable << 9;
			auto whitePawnLocation = validEnPassantMask << 8;
			if (not checkIfUnderCheckAfterEnPassant(pieceColor, board, whitePawnLocation, blackBeatingPawnLocation, validEnPassantMask))
			{
				moveTables[moveTablesN++] = MoveTable{MoveTable::Type::PawnBeatingsToLeft, NOT_RELEVANT, moveTable};
			}
		} 
		
		beatingsCandidates = ((NOT_H_COL & pawnsToMoveToLeftTop) >> 7);
		moveTable = beatingsCandidates & validEnPassantMask;
		if (moveTable)
		{
			auto blackBeatingPawnLocation = moveTable << 7;
			auto whitePawnLocation = validEnPassantMask << 8;
			if (not checkIfUnderCheckAfterEnPassant(pieceColor, board, whitePawnLocation, blackBeatingPawnLocation, validEnPassantMask))
			{
				moveTables[moveTablesN++] = MoveTable{MoveTable::Type::PawnBeatingsToRight, NOT_RELEVANT, moveTable};
			}
		}
	}
}

void MoveGeneratorV2::evaluateRocks(uint64_t rocksBitMask, const uint64_t allOccupiedFields, const uint64_t forbidenFields)
{
    for (; rocksBitMask; rocksBitMask = _blsr_u64(rocksBitMask))
    {
        unsigned char piecePosition = _tzcnt_u64(rocksBitMask);

        auto moveTable = rockBb.getAttacksFor(piecePosition, allOccupiedFields) & (~forbidenFields);
		moveTables[moveTablesN++] = MoveTable{MoveTable::Type::RockMoves, piecePosition, moveTable};
    }
}

void MoveGeneratorV2::evaluatePinnedRocks(uint64_t rocksBitMask, const uint64_t allOccupiedFields, const uint64_t forbidenFields)
{
    for (; rocksBitMask; rocksBitMask = _blsr_u64(rocksBitMask))
    {
        unsigned char piecePosition = _tzcnt_u64(rocksBitMask);

        auto moveTable = rockBb.getAttacksFor(piecePosition, allOccupiedFields) & (~forbidenFields) & PinnedRegister[piecePosition];
		moveTables[moveTablesN++] = MoveTable{MoveTable::Type::RockMoves, piecePosition, moveTable};
    }
}

void MoveGeneratorV2::evaluateBishops(uint64_t bishopsBitMask, const uint64_t allOccupiedFields, const uint64_t forbidenFields)
{
    for (; bishopsBitMask; bishopsBitMask = _blsr_u64(bishopsBitMask))
    {
        unsigned char piecePosition = _tzcnt_u64(bishopsBitMask);

        auto moveTable = bishopBb.getAttacksFor(piecePosition, allOccupiedFields) & (~forbidenFields);
		moveTables[moveTablesN++] = MoveTable{MoveTable::Type::BishopMoves, piecePosition, moveTable};
    }
}

void MoveGeneratorV2::evaluatePinnedBishops(uint64_t bishopsBitMask, const uint64_t allOccupiedFields, const uint64_t forbidenFields)
{
    for (; bishopsBitMask; bishopsBitMask = _blsr_u64(bishopsBitMask))
    {
        unsigned char piecePosition = _tzcnt_u64(bishopsBitMask);

        auto moveTable = bishopBb.getAttacksFor(piecePosition, allOccupiedFields) & (~forbidenFields) & PinnedRegister[piecePosition];
		moveTables[moveTablesN++] = MoveTable{MoveTable::Type::BishopMoves, piecePosition, moveTable};
    }
}

void MoveGeneratorV2::evaluateQueen(uint64_t queensMask, const uint64_t allOccupiedFields, const uint64_t forbidenFields)
{
    for (; queensMask; queensMask = _blsr_u64(queensMask))
    {
        unsigned char  piecePosition = _tzcnt_u64(queensMask);

        uint64_t moveTable = (rockBb.getAttacksFor(piecePosition, allOccupiedFields)
                | bishopBb.getAttacksFor(piecePosition, allOccupiedFields)) & (~forbidenFields);;

		moveTables[moveTablesN++] = {MoveTable::Type::QueenMoves, piecePosition, moveTable};
    }
}

void MoveGeneratorV2::evaluatePinnedQueen(uint64_t queensMask, const uint64_t allOccupiedFields, const uint64_t forbidenFields)
{
    for (; queensMask; queensMask = _blsr_u64(queensMask))
    {
        unsigned char  piecePosition = _tzcnt_u64(queensMask);

        uint64_t moveTable = (rockBb.getAttacksFor(piecePosition, allOccupiedFields)
                | bishopBb.getAttacksFor(piecePosition, allOccupiedFields)) & (~forbidenFields) & PinnedRegister[piecePosition];;

		moveTables[moveTablesN++] = MoveTable{MoveTable::Type::QueenMoves, piecePosition, moveTable};
    }
}

void MoveGeneratorV2::evaluateCasles(const uint64_t allOccupiedFields)
{
    if (pieceColor == NOTATION::COLOR::color::white)
    {
    	constexpr auto index = static_cast<unsigned char>(NOTATION::COLOR::color::white);
	    if (board.piecesBitSets[index].kingsMask == (1ull << 4))
	    {
	    	uint64_t castleMask = 0ull;
	        using namespace NOTATION::CASTLING_RIGHTS;
	        if (board.castlingRights & WHITE_LONG_BIT
	            && (board.piecesBitSets[index].rocksMask & (1ull))
	            && (allOccupiedFields & ((1ull << 3) | (1ull << 2) | (1ull << 1))) == 0
	            && ! CheckChecker::isAttackedOn(board, NOTATION::COLOR::color::white, 3)
	            && ! CheckChecker::isAttackedOn(board, NOTATION::COLOR::color::white, 2))
	        {
	        	castleMask |= (1ull << 2);
	        }

	        if (board.castlingRights & WHITE_SHORT_BIT
	            && (board.piecesBitSets[index].rocksMask & (1ull << 7))
	            && (allOccupiedFields & ((1ull << 5) | (1ull << 6))) == 0
	            && ! CheckChecker::isAttackedOn(board, NOTATION::COLOR::color::white, 5)
	            && ! CheckChecker::isAttackedOn(board, NOTATION::COLOR::color::white, 6))
	        {
	            castleMask |=  (1ull << 6);
	        }
       		moveTables[moveTablesN++] = MoveTable{MoveTable::Type::Castle, 4, castleMask};
	    }
    }
    else
    {
    	constexpr auto index = static_cast<unsigned char>(NOTATION::COLOR::color::black);
	    if (board.piecesBitSets[index].kingsMask == (1ull << 60))
	    {
	    	uint64_t castleMask = 0ull;
	        using namespace NOTATION::CASTLING_RIGHTS;
	        if (board.castlingRights & BLACK_LONG_BIT
	            && (board.piecesBitSets[index].rocksMask & (1ull << 56))
	            && (allOccupiedFields & ((1ull << 59) | (1ull << 58) | (1ull << 57))) == 0
	            && ! CheckChecker::isAttackedOn(board, NOTATION::COLOR::color::black, 59)
	            && ! CheckChecker::isAttackedOn(board, NOTATION::COLOR::color::black, 58))
	        {
	            castleMask |=  (1ull << 58);
	        }

	        if (board.castlingRights & BLACK_SHORT_BIT
	            && (board.piecesBitSets[index].rocksMask & (1ull << 63))
	            && (allOccupiedFields & ((1ull << 61) | (1ull << 62))) == 0
	            && ! CheckChecker::isAttackedOn(board, NOTATION::COLOR::color::black, 61)
	            && ! CheckChecker::isAttackedOn(board, NOTATION::COLOR::color::black, 62))
	        {
	            castleMask |=  (1ull << 62);
	        }
	        moveTables[moveTablesN++] = MoveTable{MoveTable::Type::Castle, 60, castleMask};
	    }
    }
}

void MoveGeneratorV2::calculateMoveTables()
{
    const auto pinnedFields = findPinned(board, pieceColor, kingPosition);
    const auto ownFields = getAllOccupiedPerColor(board, pieceColor);
    oponentFields = getAllOccupiedPerColor(board, pieceColor+1);
    const auto allOccupiedFields = ownFields | oponentFields;

    const auto forbidenForKing = ownFields | fieldsSeenByOpponent;

    if (kingAttackersCount)
    {
    	if (kingAttackersCount == 1)
    	{
    		const auto forbidenFieldsKnight = ownFields | (~possibleBlockersMask);
    		const auto notPinnedKnights = board.piecesBitSets[static_cast<unsigned char>(pieceColor)].knightsMask & (~pinnedFields.allPinned);
        	evaluateKnights(notPinnedKnights, forbidenFieldsKnight);
        	evaluateKing(forbidenForKing);

        	const auto pawns = board.piecesBitSets[static_cast<unsigned char>(pieceColor)].pawnsMask;
        	const auto pawnsToMoveVertically = pawns & (~(pinnedFields.allPinned ^ pinnedFields.verticallyPinned));

	        evaluatePawns(pawnsToMoveVertically, allOccupiedFields, possibleBlockersMask);
	        const auto pawnsMovingOnRightTopDiagonal = pawns & ((~pinnedFields.allPinned) | pinnedFields.diagonallyPinnedFromLeftBottom);
	        const auto pawnsMovingOnLeftTopDiagonal = pawns & ((~pinnedFields.allPinned) | pinnedFields.diagonallyPinnedFromLeftTop);

            evaluatePawnsBeatings(pawnsMovingOnRightTopDiagonal, pawnsMovingOnLeftTopDiagonal, (oponentFields & possibleBlockersMask));
        	evaluateEnPassant(pawnsMovingOnRightTopDiagonal, pawnsMovingOnLeftTopDiagonal);

	        const auto rocks = board.piecesBitSets[static_cast<unsigned char>(pieceColor)].rocksMask;
	        evaluateRocks(rocks & (~pinnedFields.allPinned), allOccupiedFields, ownFields | (~possibleBlockersMask));

	        const auto horizontalyAndVerticallyPinned = pinnedFields.verticallyPinned | pinnedFields.horizontallyPinned;
	        evaluatePinnedRocks(rocks & horizontalyAndVerticallyPinned, allOccupiedFields, ownFields | (~possibleBlockersMask));

	        const auto bishops = board.piecesBitSets[static_cast<unsigned char>(pieceColor)].bishopsMask;
	        evaluateBishops(bishops & (~pinnedFields.allPinned), allOccupiedFields, ownFields | (~possibleBlockersMask));

        	const auto diagonallyPined = pinnedFields.diagonallyPinnedFromLeftBottom | pinnedFields.diagonallyPinnedFromLeftTop;
        	evaluatePinnedBishops(bishops & diagonallyPined, allOccupiedFields, ownFields | (~possibleBlockersMask));

        	const auto queens = board.piecesBitSets[static_cast<unsigned char>(pieceColor)].queensMask;
			evaluateQueen(queens & (~pinnedFields.allPinned), allOccupiedFields, ownFields | (~possibleBlockersMask));
        	evaluatePinnedQueen(queens & pinnedFields.allPinned, allOccupiedFields, ownFields | (~possibleBlockersMask));
        	return;
    	}
    	else
    	{
    		evaluateKing(forbidenForKing);
    		return;
    	}
    }
    else
    {
		const auto pawns = board.piecesBitSets[static_cast<unsigned char>(pieceColor)].pawnsMask;
    	const auto notPinnedKnights = board.piecesBitSets[static_cast<unsigned char>(pieceColor)].knightsMask & (~pinnedFields.allPinned);
        evaluateKnights(notPinnedKnights, ownFields);
        evaluateKing(forbidenForKing);

        const auto pawnsToMoveVertically = pawns & (~(pinnedFields.allPinned ^ pinnedFields.verticallyPinned));
        evaluatePawns(pawnsToMoveVertically, allOccupiedFields);

        const auto pawnsMovingOnRightTopDiagonal = pawns & ((~pinnedFields.allPinned) | pinnedFields.diagonallyPinnedFromLeftBottom);
        const auto pawnsMovingOnLeftTopDiagonal = pawns & ((~pinnedFields.allPinned) | pinnedFields.diagonallyPinnedFromLeftTop);
        evaluatePawnsBeatings(pawnsMovingOnRightTopDiagonal, pawnsMovingOnLeftTopDiagonal, oponentFields);
        evaluateEnPassant(pawnsMovingOnRightTopDiagonal, pawnsMovingOnLeftTopDiagonal);

        const auto rocks = board.piecesBitSets[static_cast<unsigned char>(pieceColor)].rocksMask;
        evaluateRocks(rocks & (~pinnedFields.allPinned), allOccupiedFields, ownFields);

        const auto horizontalyAndVerticallyPinned = pinnedFields.verticallyPinned | pinnedFields.horizontallyPinned;
        evaluatePinnedRocks(rocks & horizontalyAndVerticallyPinned, allOccupiedFields, ownFields);

        const auto bishops = board.piecesBitSets[static_cast<unsigned char>(pieceColor)].bishopsMask;
        evaluateBishops(bishops & (~pinnedFields.allPinned), allOccupiedFields, ownFields);

        const auto diagonallyPined = pinnedFields.diagonallyPinnedFromLeftBottom | pinnedFields.diagonallyPinnedFromLeftTop;
        evaluatePinnedBishops(bishops & diagonallyPined, allOccupiedFields, ownFields);

		const auto queens = board.piecesBitSets[static_cast<unsigned char>(pieceColor)].queensMask;
		evaluateQueen(queens & (~pinnedFields.allPinned), allOccupiedFields, ownFields);
        evaluatePinnedQueen(queens & pinnedFields.allPinned, allOccupiedFields, ownFields);
        evaluateCasles(allOccupiedFields);
        return;
    }
}

void MoveGeneratorV2::calculateMoveCount()
{
	moveCount = 0;
	for (auto* mtPtr= moveTables; mtPtr != moveTables + moveTablesN; ++mtPtr)
	{
		moveCount += __builtin_popcountll(mtPtr->bitField);
	}
}

template <NOTATION::COLOR::color c>
ExtendedMove createPawnBeatingMove(const Board& board, unsigned char source, unsigned char destination)
{
    constexpr unsigned char pawn = NOTATION::PIECES::PAWN | static_cast<unsigned char>(c);

	unsigned char MASK = ExtendedMove::pawnMoveMask | ExtendedMove::beatingMask;
	auto targetPiece = board.getField(destination);
	if (targetPiece == 0)
	{
		MASK |= ExtendedMove::enPasantMask;
		targetPiece = NOTATION::PIECES::PAWN | static_cast<unsigned char>(c+1);
	}
	ExtendedMove m = ExtendedMove(source, destination, MASK, 0, pawn, targetPiece);
	return m;
}

template <NOTATION::COLOR::color c>
void fillPawnBeatingPromotionMoves(TBeatingVector& beatingMoves,
	TNormalVector& normalMoves, const Board& board, unsigned char source, unsigned char destination)
{
    constexpr unsigned char pawn = NOTATION::PIECES::PAWN | static_cast<unsigned char>(c);
	constexpr unsigned char c_bin = static_cast<unsigned char>(c);
	constexpr unsigned char MASK = ExtendedMove::pawnMoveMask | ExtendedMove::beatingMask |  ExtendedMove::promotionMask;
	auto targetPiece = board.getFieldForNonEmpty(destination, c+1);

	beatingMoves.emplace_back(source, destination, MASK, NOTATION::PIECES::QUEEN | c_bin, pawn, targetPiece);
// Very unexplicit. To be organized somehow better.
	normalMoves.emplace_back(source, destination, MASK, NOTATION::PIECES::BISHOP | c_bin, pawn, targetPiece);
	normalMoves.emplace_back(source, destination, MASK, NOTATION::PIECES::ROCK | c_bin, pawn, targetPiece);
	normalMoves.emplace_back(source, destination, MASK, NOTATION::PIECES::KNIGHT | c_bin, pawn, targetPiece);
}

template <NOTATION::COLOR::color c, uint64_t PROMOTION_LINE, signed char FromTargetToSourceDiff>
void MoveGeneratorV2::fillPawnBeatingsMoves(uint64_t bitField)
{
	auto destinationsNonPromotions = bitField & (~PROMOTION_LINE);
    for (; destinationsNonPromotions; destinationsNonPromotions = _blsr_u64(destinationsNonPromotions))
	{
		unsigned char destination = _tzcnt_u64(destinationsNonPromotions);
		unsigned char source = destination + FromTargetToSourceDiff;

		beatingMoves.emplace_back( createPawnBeatingMove<c>(board, source, destination));
	}
    auto destinationsPromotions = bitField & PROMOTION_LINE;
    for (; destinationsPromotions; destinationsPromotions = _blsr_u64(destinationsPromotions))
	{
		unsigned char destination = _tzcnt_u64(destinationsPromotions);
		unsigned char source = destination + FromTargetToSourceDiff;

		fillPawnBeatingPromotionMoves<c>(beatingMoves, normalMoves, board, source, destination);
	}
}

template <NOTATION::COLOR::color c, signed char FromTargetToSourceDiff>
void fillPawnPromotions(TBeatingVector& beatingMoves, TNormalVector& normalMoves, uint64_t bitField)
{
    constexpr unsigned char pawn = NOTATION::PIECES::PAWN | static_cast<unsigned char>(c);
	constexpr unsigned char c_bin = static_cast<unsigned char>(c);
	constexpr unsigned char MASK = ExtendedMove::pawnMoveMask | ExtendedMove::promotionMask;
    for (; bitField; bitField = _blsr_u64(bitField))
	{
		unsigned char destination = _tzcnt_u64(bitField);
		unsigned char source = destination + FromTargetToSourceDiff;

		beatingMoves.emplace_back(source, destination, MASK, NOTATION::PIECES::QUEEN | c_bin, pawn, 0);
		// Very unexplicit. To be organized somehow better.
		normalMoves.emplace_back(source, destination, MASK, NOTATION::PIECES::BISHOP | c_bin, pawn, 0);
		normalMoves.emplace_back(source, destination, MASK, NOTATION::PIECES::ROCK | c_bin, pawn, 0);
		normalMoves.emplace_back(source, destination, MASK, NOTATION::PIECES::KNIGHT | c_bin, pawn, 0);
	}
}

void fillRockBeating(TBeatingVector& beatingMoves, const Board& board, unsigned char sourceFigure, NOTATION::COLOR::color c, unsigned char source, uint64_t validDestinations)
{
	constexpr unsigned char MASK = ExtendedMove::beatingMask | ExtendedMove::rockMoveMask;
	const unsigned char sourcePiece = sourceFigure | static_cast<unsigned char>(c);
    for (; validDestinations; validDestinations = _blsr_u64(validDestinations))
    {
        unsigned char destination = _tzcnt_u64(validDestinations);
        beatingMoves.emplace_back(source, destination, MASK, 0, sourcePiece, board.getFieldForNonEmpty(destination, c+1));
    }
}

void fillClassicBeating(TBeatingVector& beatingMoves, const Board& board, unsigned char sourceFigure, NOTATION::COLOR::color c, unsigned char source, uint64_t validDestinations)
{
	constexpr unsigned char MASK = ExtendedMove::beatingMask;
	const unsigned char sourcePiece = sourceFigure | static_cast<unsigned char>(c);

    for (; validDestinations; validDestinations = _blsr_u64(validDestinations))
    {
        unsigned char destination = _tzcnt_u64(validDestinations);
        beatingMoves.emplace_back(source, destination, MASK, 0, sourcePiece, board.getFieldForNonEmpty(destination, c+1));
    }
}

std::span<ExtendedMove> MoveGeneratorV2::generateBeatingMoves()
{
	for (auto* mtPtr= moveTables; mtPtr != moveTables + moveTablesN; ++mtPtr)
	{
		const auto mt = *mtPtr;
		if (mt.type == MoveTable::Type::KingMoves)
		{
        	constexpr unsigned char MASK = ExtendedMove::kingMoveMask | ExtendedMove::beatingMask;
        	const unsigned char king = NOTATION::PIECES::KING | static_cast<unsigned char>(pieceColor);

        	auto destinations = mt.bitField & oponentFields;
            for (; destinations; destinations = _blsr_u64(destinations))
    		{
    			unsigned char destination = _tzcnt_u64(destinations);
        		beatingMoves.emplace_back(mt.sourceField, destination, MASK, 0, king, board.getFieldForNonEmpty(destination, pieceColor+1));
    		}
    		continue;
		}
        if (mt.type == MoveTable::Type::PawnBeatingsToRight)
		{
			if (pieceColor == NOTATION::COLOR::color::white)
			{
				fillPawnBeatingsMoves<NOTATION::COLOR::color::white, 0xFF'00'00'00'00'00'00'00ull, -9>(mt.bitField);
			}
			else
			{
				fillPawnBeatingsMoves<NOTATION::COLOR::color::black, 0x00'00'00'00'00'00'00'FFull, 7>(mt.bitField);
			}
			continue;
		}
        if (mt.type == MoveTable::Type::PawnBeatingsToLeft)
		{
			if (pieceColor == NOTATION::COLOR::color::white)
			{
				fillPawnBeatingsMoves<NOTATION::COLOR::color::white, 0xFF'00'00'00'00'00'00'00ull, -7>(mt.bitField);
			}
			else
			{
				fillPawnBeatingsMoves<NOTATION::COLOR::color::black, 0x00'00'00'00'00'00'00'FFull, 9>(mt.bitField);
			}
			continue;
		}
		if (mt.type == MoveTable::Type::SinglePawnMoves)  // just promotions
		{
			if (pieceColor == NOTATION::COLOR::color::white)
			{
				fillPawnPromotions<NOTATION::COLOR::color::white, -8>(beatingMoves, normalMoves, mt.bitField & 0xFF'00'00'00'00'00'00'00ull);
			}
			else
			{
				fillPawnPromotions<NOTATION::COLOR::color::black, 8>(beatingMoves, normalMoves, mt.bitField & 0x00'00'00'00'00'00'00'FFull);
			}
			continue;
		}
		if (mt.type == MoveTable::Type::RockMoves)
		{
			fillRockBeating(beatingMoves, board, static_cast<unsigned char>(mt.type), pieceColor, mt.sourceField, mt.bitField & oponentFields);
		}
		if ((mt.type == MoveTable::Type::KnightMoves) | (mt.type == MoveTable::Type::BishopMoves) | (mt.type == MoveTable::Type::QueenMoves)) 
		{
			fillClassicBeating(beatingMoves, board, static_cast<unsigned char>(mt.type), pieceColor, mt.sourceField, mt.bitField & oponentFields);
		}
	}
	return beatingMoves;
}

template <NOTATION::COLOR::color c, signed char FromTargetToSourceDiff>
void fillPawn(TNormalVector& normalMoves, uint64_t destinations)
{
    constexpr unsigned char pawn = NOTATION::PIECES::PAWN | static_cast<unsigned char>(c);
	constexpr unsigned char MASK = ExtendedMove::pawnMoveMask;

    for (; destinations; destinations = _blsr_u64(destinations))
    {
		unsigned char destination = _tzcnt_u64(destinations);
		unsigned char source = destination + FromTargetToSourceDiff;

		normalMoves.emplace_back(source, destination, MASK, 0, pawn, 0);
	}
}

template<NOTATION::COLOR::color c>
void addCastling(TNormalVector& normalMoves, unsigned char source, unsigned char destination)
{
    constexpr unsigned char MASK = ExtendedMove::kingMoveMask | ExtendedMove::castlingMask;
    constexpr unsigned char king = NOTATION::PIECES::KING | static_cast<unsigned char>(c);

    normalMoves.emplace_back(source, destination, MASK, 0, king, 0);
}

template<NOTATION::COLOR::color c>
void fillRockMove(TNormalVector& normalMoves, unsigned char source, uint64_t validDestinations)
{
	constexpr unsigned char sourcePiece = NOTATION::PIECES::ROCK | static_cast<unsigned char>(c);
    for (; validDestinations; validDestinations = _blsr_u64(validDestinations))
    {
		unsigned char destination = _tzcnt_u64(validDestinations);
		normalMoves.emplace_back(source, destination, ExtendedMove::rockMoveMask, 0, sourcePiece, 0);
	}
}

inline void fillNormalMove(TNormalVector& normalMoves, unsigned char sourceFigure, NOTATION::COLOR::color c, unsigned char source, uint64_t validDestinations)
{
	const unsigned char sourcePiece = sourceFigure | static_cast<unsigned char>(c);
    for (; validDestinations; validDestinations = _blsr_u64(validDestinations))
	{
		unsigned char destination = _tzcnt_u64(validDestinations);
		normalMoves.emplace_back(source, destination, 0, 0, sourcePiece, 0);
	}
}

std::span<ExtendedMove> MoveGeneratorV2::generateNonBeatingMoves()
{
    if (pieceColor == NOTATION::COLOR::color::white)
    {
        return generateNonBeatingMovesImpl<NOTATION::COLOR::color::white>();
    }
    return generateNonBeatingMovesImpl<NOTATION::COLOR::color::black>();
}

template<NOTATION::COLOR::color TColor>
std::span<ExtendedMove> MoveGeneratorV2::generateNonBeatingMovesImpl()
{
	const auto emptyFields = ~getAllOccupiedFields(board);

	for (auto* mtPtr= moveTables; mtPtr != moveTables + moveTablesN; ++mtPtr)
	{
		const auto& mt = *mtPtr;
		if (mt.type == MoveTable::Type::KingMoves)
		{
        	constexpr unsigned char MASK = ExtendedMove::kingMoveMask;
            constexpr unsigned char king = NOTATION::PIECES::KING | static_cast<unsigned char>(TColor);

        	auto destinations = mt.bitField & emptyFields;
            for (; destinations; destinations = _blsr_u64(destinations))
    		{
    			unsigned char destination = _tzcnt_u64(destinations);
        		normalMoves.emplace_back(mt.sourceField, destination, MASK, 0, king, 0);
    		}
    		continue;
		}
		if (mt.type == MoveTable::Type::SinglePawnMoves)  // nonPromotions
		{
			if constexpr (TColor == NOTATION::COLOR::color::white)
			{
				fillPawn<NOTATION::COLOR::color::white, -8>(normalMoves, mt.bitField & 0x00'FF'FF'FF'FF'FF'FF'FFull);
			}
			else
			{
				fillPawn<NOTATION::COLOR::color::black, 8>(normalMoves, mt.bitField & 0xFF'FF'FF'FF'FF'FF'FF'00ull);
			}
			continue;
		}
		if (mt.type == MoveTable::Type::DoublePawnMoves)  // nonPromotions
		{
			if constexpr (TColor == NOTATION::COLOR::color::white)
			{
				fillPawn<NOTATION::COLOR::color::white, -16>(normalMoves, mt.bitField);
			}
			else
			{
				fillPawn<NOTATION::COLOR::color::black, 16>(normalMoves, mt.bitField);
			}
			continue;
		}
		if (mt.type == MoveTable::Type::Castle)
		{
			if constexpr (TColor == NOTATION::COLOR::color::white)
			{
				if (mt.bitField & (1ull << 2))
				{
					addCastling<NOTATION::COLOR::color::white>(normalMoves, 4, 2);
				}
				if (mt.bitField & (1ull << 6))
				{
					addCastling<NOTATION::COLOR::color::white>(normalMoves, 4, 6);
				}
			}
	        else
			{
                if (mt.bitField & (1ull << 58))
				{
                    addCastling<NOTATION::COLOR::color::black>(normalMoves, 60, 58);
				}
				if (mt.bitField & (1ull << 62))
				{
                    addCastling<NOTATION::COLOR::color::black>(normalMoves, 60, 62);
				}
			}
			continue;
		}
		if (mt.type == MoveTable::Type::RockMoves)
		{
			fillRockMove<TColor>(normalMoves, mt.sourceField, mt.bitField & emptyFields);
		}
		if ((mt.type == MoveTable::Type::KnightMoves) | (mt.type == MoveTable::Type::BishopMoves) | (mt.type == MoveTable::Type::QueenMoves)) 
		{
			fillNormalMove(normalMoves, static_cast<unsigned char>(mt.type), TColor, mt.sourceField, mt.bitField & emptyFields);
		}
	}
	return normalMoves;
}

}  // namespace MoveGenerator
