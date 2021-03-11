#pragma once

#include <cstdint>
#include <detail/bitboardslookups.hpp>
#include <detail/BitBoardsUtils.h>
#include <publicIf/Notation.hpp>
#include <publicIf/Board.hpp>

uint64_t getAttackedFieldsBy(const Board& board, const NOTATION::COLOR::color c, uint64_t blockers)
{
    auto c_num = static_cast<unsigned char>(c);
    uint64_t attackedFields = 0ull;
    // pawnsAttacks
    if (c == NOTATION::COLOR::color::white)
    {
        attackedFields |= (board.piecesBitSets[c_num].pawnsMask & NOT_H_COL) << 9;
        attackedFields |= (board.piecesBitSets[c_num].pawnsMask & NOT_A_COL) << 7;
    }
    if (c == NOTATION::COLOR::color::black)
    {
        attackedFields |= (board.piecesBitSets[c_num].pawnsMask & NOT_H_COL) >> 7;
        attackedFields |= (board.piecesBitSets[c_num].pawnsMask & NOT_A_COL) >> 9;
    }

    auto knightsBitMask = board.piecesBitSets[c_num].knightsMask;
    while (knightsBitMask)
    {
        auto knightIndex = 63 - __builtin_clzll(knightsBitMask);
        knightsBitMask ^= 1ull << knightIndex;

        attackedFields |= bitBoardLookup[knightIndex].knightsMovePossibilities;
    }

    auto kingIndex = 63 - __builtin_clzll(board.piecesBitSets[c_num].kingsMask);
    attackedFields |= bitBoardLookup[kingIndex].kingMovePossibilities;

    auto diagonalBitMask = board.piecesBitSets[c_num].bishopsMask | board.piecesBitSets[c_num].queensMask;
    while (diagonalBitMask)
    {
        auto piecePosition = 63 - __builtin_clzll(diagonalBitMask);
        diagonalBitMask ^= 1ull << piecePosition;
        attackedFields |= bishopMagicBb.getAttacksFor(piecePosition, blockers);
    }

    auto lineBitMask = board.piecesBitSets[c_num].rocksMask | board.piecesBitSets[c_num].queensMask;
    while (lineBitMask)
    {
        auto piecePosition = 63 - __builtin_clzll(lineBitMask);
        lineBitMask ^= 1ull << piecePosition;
        attackedFields |= rockMagicBb.getAttacksFor(piecePosition, blockers);
    }
    return attackedFields;
}