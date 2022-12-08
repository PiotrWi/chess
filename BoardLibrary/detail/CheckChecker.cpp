#include "CheckChecker.hpp"

#include <algorithm>
#include <publicIf/Board.hpp>
#include <publicIf/NotationConversions.hpp>

#include <detail/BitBoardsUtils.h>
#include <detail/bitboardslookups.hpp>

namespace CheckChecker
{

bool isAttackedBySliders(const Board& board,
                                 NOTATION::COLOR::color playerColor,
                                 unsigned char field) noexcept
{
    auto oppositeColorNum = static_cast<unsigned char>(playerColor + 1);

    auto allPieces = getAllOccupiedFields(board);

    auto OppositeQueenAndRock =
            board.piecesBitSets[oppositeColorNum].queensMask
            | board.piecesBitSets[oppositeColorNum].rocksMask;

    uint64_t rockOrQueenAttackers = rockBb.getAttacksFor(field, allPieces) & OppositeQueenAndRock;

    auto OppositeQueenAndBishop =
            board.piecesBitSets[oppositeColorNum].queensMask
            | board.piecesBitSets[oppositeColorNum].bishopsMask;
    uint64_t bishopOrQueenAttackers = bishopBb.getAttacksFor(field, allPieces) & OppositeQueenAndBishop;

    return rockOrQueenAttackers | bishopOrQueenAttackers;
}

bool isAttackedOn(const Board& board,
		NOTATION::COLOR::color playerColor,
		unsigned char fieldPosition) noexcept
{
    auto oppositeColorNum = static_cast<unsigned char>(playerColor + 1);
    auto allOppositePawns = board.piecesBitSets[oppositeColorNum].pawnsMask;
    auto attackingPawn = allOppositePawns & getPawnAttacks(playerColor, fieldPosition);
    const auto& lookup = bitBoardLookup[fieldPosition];

    auto oppositeKing = board.piecesBitSets[oppositeColorNum].kingsMask;
    auto attackingKings = oppositeKing & lookup.kingMovePossibilities;

    auto allOppositeKnights = board.piecesBitSets[oppositeColorNum].knightsMask;
    auto attackingKnights = allOppositeKnights & lookup.knightsMovePossibilities;

    auto allPieces = getAllOccupiedFields(board);

    auto OppositeQueenAndRock =
            board.piecesBitSets[oppositeColorNum].queensMask
            | board.piecesBitSets[oppositeColorNum].rocksMask;

    uint64_t rockOrQueenAttackers = rockBb.getAttacksFor(fieldPosition, allPieces) & OppositeQueenAndRock;

    auto OppositeQueenAndBishop =
            board.piecesBitSets[oppositeColorNum].queensMask
            | board.piecesBitSets[oppositeColorNum].bishopsMask;
    uint64_t bishopOrQueenAttackers = bishopBb.getAttacksFor(fieldPosition, allPieces) & OppositeQueenAndBishop;

    return attackingPawn | attackingKnights | attackingKings | rockOrQueenAttackers | bishopOrQueenAttackers;
}

AttackOut isCheckedBeforeMoveExtendeded(const Board& board,
        NOTATION::COLOR::color playerColor,
        unsigned char fieldPosition) noexcept
{
    AttackOut out;

    auto oppositeColorNum = static_cast<unsigned char>(playerColor + 1);
    auto allOppositePawns = board.piecesBitSets[oppositeColorNum].pawnsMask;
    auto attackingPawn = allOppositePawns & getPawnAttacks(playerColor, fieldPosition);

    const auto& lookup = bitBoardLookup[fieldPosition];
    auto allOppositeKnights = board.piecesBitSets[oppositeColorNum].knightsMask;
    auto attackingKnights = allOppositeKnights & lookup.knightsMovePossibilities;

    auto allPieces = getAllOccupiedFields(board);

    auto OppositeQueenAndRock =
            board.piecesBitSets[oppositeColorNum].queensMask
            | board.piecesBitSets[oppositeColorNum].rocksMask;
    uint64_t lineAttacksFromField = rockBb.getAttacksFor(fieldPosition, allPieces);
    uint64_t rockOrQueenAttackers = lineAttacksFromField & OppositeQueenAndRock;
    if (rockOrQueenAttackers)
    {
        auto attackerField = _tzcnt_u64(rockOrQueenAttackers);
        uint64_t attackersBBs = rockBb.getAttacksFor(attackerField, allPieces);
        out.possibleBlockersMask |= lineAttacksFromField & attackersBBs;
    }

    auto OppositeQueenAndBishop =
            board.piecesBitSets[oppositeColorNum].queensMask
            | board.piecesBitSets[oppositeColorNum].bishopsMask;
    uint64_t diagonalAttacksFromField = bishopBb.getAttacksFor(fieldPosition, allPieces);
    uint64_t bishopOrQueenAttackers = diagonalAttacksFromField & OppositeQueenAndBishop;
    if (bishopOrQueenAttackers)
    {
        auto attackerField = _tzcnt_u64(bishopOrQueenAttackers);
        uint64_t attackersBBs = bishopBb.getAttacksFor(attackerField, allPieces);
        out.possibleBlockersMask |= diagonalAttacksFromField & attackersBBs;
    }
    
    auto allAttackers = attackingPawn | attackingKnights | bishopOrQueenAttackers | rockOrQueenAttackers;
    out.possibleBlockersMask |= (allAttackers);
    out.attackersNum = __builtin_popcountll(allAttackers);

    return out;
}


unsigned char findKing(const Board& board, const NOTATION::COLOR::color c) noexcept
{
    return _tzcnt_u64(board.piecesBitSets[static_cast<unsigned char>(c)].kingsMask);
}

bool isCheckOn(const Board& board, const NOTATION::COLOR::color c) noexcept
{
    unsigned char kingPos = _tzcnt_u64(board.piecesBitSets[static_cast<unsigned char>(c)].kingsMask);

    return isAttackedOn(board, c, kingPos);
}

}  // namespace CheckChecker
