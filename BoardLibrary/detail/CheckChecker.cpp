#include "CheckChecker.hpp"

#include <algorithm>
#include <array>
#include <publicIf/Board.hpp>
#include <publicIf/NotationConversions.hpp>

#include <detail/BitBoardsUtils.h>
#include <detail/bitboardslookups.hpp>

namespace CheckChecker
{

bool isAttackedOn(const Board& board,
		NOTATION::COLOR::color playerColor,
		unsigned char fieldPosition) noexcept
{
    const auto& lookup = bitBoardLookup[fieldPosition];

    auto oppositeColorNum = static_cast<unsigned char>(playerColor + 1);

    auto allOppositePawns = board.piecesBitSets[oppositeColorNum].pawnsMask;
    uint64_t attackingPawn;
    if (playerColor == NOTATION::COLOR::color::white)
    {
        attackingPawn = lookup.OppositePawnsAttackingFieldForWhite & allOppositePawns;
    }
    else
    {
        attackingPawn = lookup.OppositePawnsAttackingFieldForBlack & allOppositePawns;
    }

    auto allOppositeKnights = board.piecesBitSets[oppositeColorNum].knightsMask;
    auto attackingKnights = allOppositeKnights & lookup.knightsMovePossibilities;

    auto oppositeKing = board.piecesBitSets[oppositeColorNum].kingsMask;
    auto attackingKings = oppositeKing & lookup.kingMovePossibilities;

    auto allPieces = getAllOccupiedFields(board);

    auto OppositeQueenAndRock =
            board.piecesBitSets[oppositeColorNum].queensMask
            | board.piecesBitSets[oppositeColorNum].rocksMask;

    uint64_t rockOrQueenAttackers = rockMagicBb.getAttacksFor(fieldPosition, allPieces) & OppositeQueenAndRock;

    auto OppositeQueenAndBishop =
            board.piecesBitSets[oppositeColorNum].queensMask
            | board.piecesBitSets[oppositeColorNum].bishopsMask;
    uint64_t bishopOrQueenAttackers = bishopMagicBb.getAttacksFor(fieldPosition, allPieces) & OppositeQueenAndBishop;

    return attackingPawn | attackingKnights | attackingKings | rockOrQueenAttackers | bishopOrQueenAttackers;
}

unsigned char findKing(const Board& board, const NOTATION::COLOR::color c) noexcept
{
    return 63 - __builtin_clzll(board.piecesBitSets[static_cast<unsigned char>(c)].kingsMask);
}

bool isCheckOn(const Board& board, const NOTATION::COLOR::color c) noexcept
{
    unsigned char kingPos = 63 - __builtin_clzll(board.piecesBitSets[static_cast<unsigned char>(c)].kingsMask);

    return isAttackedOn(board, c, kingPos);
}

}  // namespace CheckChecker
