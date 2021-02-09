#include "CheckChecker.hpp"

#include <algorithm>
#include <publicIf/Board.hpp>
#include <publicIf/NotationConversions.hpp>

#include <FieldLookup/FieldLookup.hpp>

namespace CheckChecker
{

constexpr uint64_t NOT_H_COL = 0x8f'ff'ff'ff'ff'ff'ff'ffull;
constexpr uint64_t NOT_HG_COL = 0x4f'ff'ff'ff'ff'ff'ff'ffull;
constexpr uint64_t NOT_A_COL = 0xff'ff'ff'ff'ff'ff'ff'fdull;
constexpr uint64_t NOT_AB_COL = 0xff'ff'ff'ff'ff'ff'ff'fbull;


// TODO: All of these bitmast shall be precalculated once
bool isAttackedOn(const Board& board,
		NOTATION::COLOR::color playerColor,
		unsigned char fieldPosition) noexcept
{
    auto oppositeColorNum = static_cast<unsigned char>(playerColor + 1);

    auto kingBitMask = (1ull << fieldPosition);
    auto allOppositePawns = board.piecesBitSets[oppositeColorNum].pawnsMask;

    uint64_t oppositeAttackingPawnsCandidates;
    if (playerColor == NOTATION::COLOR::color::white)
    {
        oppositeAttackingPawnsCandidates =
                (NOT_H_COL & kingBitMask) << 7
                | (NOT_A_COL & kingBitMask) << 9;
    }
    else
    {
        oppositeAttackingPawnsCandidates =
                (NOT_H_COL & kingBitMask) >> 9
                | (NOT_A_COL & kingBitMask) >> 7;
    }
    auto attackingPawn = oppositeAttackingPawnsCandidates & allOppositePawns;

    auto allOppositeKnights = board.piecesBitSets[oppositeColorNum].knightsMask;
    uint64_t oppositeAttackingKnightsCandidates =
            (NOT_HG_COL & kingBitMask) >> 10
            | (NOT_HG_COL & kingBitMask) << 6
            | (NOT_H_COL & kingBitMask) << 15
            | (NOT_H_COL & kingBitMask) >> 17
            | (NOT_AB_COL & kingBitMask) >> 6
            | (NOT_AB_COL & kingBitMask) << 10
            | (NOT_A_COL & kingBitMask) << 17
            | (NOT_A_COL & kingBitMask) >> 15;
    auto attackingKnights = allOppositeKnights & oppositeAttackingKnightsCandidates;

    auto oppositeKing = board.piecesBitSets[oppositeColorNum].kingsMask;
    uint64_t oppositeAttackingKingCandidates =
            kingBitMask << 8
            | kingBitMask >> 8
            | (NOT_A_COL & kingBitMask) << 7
            | (NOT_A_COL & kingBitMask) >> 1
            | (NOT_A_COL & kingBitMask) >> 9
            | (NOT_H_COL & kingBitMask) << 9
            | (NOT_H_COL & kingBitMask) << 1
            | (NOT_H_COL & kingBitMask) >> 7;
    auto attackingKings = oppositeKing & oppositeAttackingKingCandidates;

    auto OppositeQueenAndRock =
            board.piecesBitSets[oppositeColorNum].queensMask
            | board.piecesBitSets[oppositeColorNum].rocksMask;

    auto allPieces = board.piecesBitSets[0].rocksMask
            | board.piecesBitSets[0].queensMask
            | board.piecesBitSets[0].kingsMask
            | board.piecesBitSets[0].knightsMask
            | board.piecesBitSets[0].pawnsMask
            | board.piecesBitSets[0].bishopsMask
            | board.piecesBitSets[1].rocksMask
            | board.piecesBitSets[1].queensMask
            | board.piecesBitSets[1].kingsMask
            | board.piecesBitSets[1].knightsMask
            | board.piecesBitSets[1].pawnsMask
            | board.piecesBitSets[1].bishopsMask;

    // NORTH
    auto northMask = kingBitMask << 8;
    northMask |= northMask << 8;
    northMask |= northMask << 16;
    northMask |= northMask << 32;

    auto northPieces = northMask & allPieces;
    auto northRockOrQueenAttacker = (1ull << (__builtin_ffsll(northPieces) - 1)) & OppositeQueenAndRock;

    // RIGHT
    uint64_t rightMask = 0ull;
    for (auto i = fieldPosition % 8 + 1; i < 8; ++i)
    {
        rightMask |= (rightMask << 1) | (kingBitMask << 1);
    }

    auto rightPieces = rightMask & allPieces;
    auto rightRockOrQueenAttacker = (1ull << (__builtin_ffsll(rightPieces) - 1)) & OppositeQueenAndRock;

    return attackingPawn | attackingKnights | attackingKings | northRockOrQueenAttacker | rightRockOrQueenAttacker;
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
