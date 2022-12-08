#pragma once

#include <publicIf/Board.hpp>
#include <immintrin.h>

constexpr uint64_t NOT_H_COL = 0x7f'7f'7f'7f'7f'7f'7f'7full;
constexpr uint64_t NOT_HG_COL = 0x3f'3f'3f'3f'3f'3f'3f'3full;
constexpr uint64_t NOT_A_COL = 0xfe'fe'fe'fe'fe'fe'fe'feull;
constexpr uint64_t NOT_AB_COL = 0xfc'fc'fc'fc'fc'fc'fc'fcull;

constexpr int getOccupiedBitIndex(const uint64_t& bitMask)
{
    return 63 - __builtin_clzll(bitMask);
}

constexpr uint64_t getAllOccupiedFields(const Board& board)
{
    return board.piecesBitSets[0].rocksMask
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
}

constexpr uint64_t getAllOccupiedPerColor(const Board& board, NOTATION::COLOR::color c)
{
    auto colorNum = static_cast<unsigned char>(c);
    return board.piecesBitSets[colorNum].rocksMask
           | board.piecesBitSets[colorNum].queensMask
           | board.piecesBitSets[colorNum].kingsMask
           | board.piecesBitSets[colorNum].knightsMask
           | board.piecesBitSets[colorNum].pawnsMask
           | board.piecesBitSets[colorNum].bishopsMask;
}



// Get all the fields to which pawn can move going forward and potentially reach by single beating.
// Place where pawn stan also is included.
//
// See bellow
// "   xxx  " "   xxxx "
// "   xxx  " "   xxxx "
// "   xxx  " "   xxxx "
// "   xxx  " "   xxxx "
// "    ♙   " "    ♙xx "
// "        " "     ♙  "
// "        " "        "
// "        " "        "
constexpr uint64_t getFullFrontSpan(const Board& board, NOTATION::COLOR::color c)
{
    auto colorNum = static_cast<unsigned char>(c);
    uint64_t out = board.piecesBitSets[colorNum].pawnsMask;
    if (c == NOTATION::COLOR::color::white)
    {
        out |= (out << 8) | ((NOT_H_COL & out) << 9) | ((NOT_A_COL & out) << 7);
        out |= out << 8;
        out |= out << 16;
        out |= out << 32;
        return out;
    }
    out |= (out >> 8) | ((NOT_H_COL & out) >> 7) | ((NOT_A_COL & out) >> 9);
    out |= out >> 8;
    out |= out >> 16;
    out |= out >> 32;
    return out;
}

constexpr uint64_t getFullRearSpan(const Board& board, NOTATION::COLOR::color c)
{
    return ~getFullFrontSpan(board, c);
}

constexpr uint64_t getPawnAttacks(const NOTATION::COLOR::color c, const unsigned char field)
{
    const auto fieldBitMap = (1ull << field);
    if (c == NOTATION::COLOR::color::white)
    {
        return ((NOT_H_COL & fieldBitMap) << 9) | ((NOT_A_COL & fieldBitMap) << 7);
    }
    return ((NOT_H_COL & fieldBitMap) >> 7) | ((NOT_A_COL & fieldBitMap) >> 9);
}

constexpr uint64_t getAllFieldsAttackedByPawns(const Board& board, NOTATION::COLOR::color c)
{
    auto colorNum = static_cast<unsigned char>(c);
    uint64_t pawns = board.piecesBitSets[colorNum].pawnsMask;

    if (c == NOTATION::COLOR::color::white)
    {
        return ((NOT_H_COL & pawns) << 9) | ((NOT_A_COL & pawns) << 7);
    }
    return ((NOT_H_COL & pawns) >> 7) | ((NOT_A_COL & pawns) >> 9);
}

constexpr uint64_t getAllFieldsAttackedByKing(const Board& board, NOTATION::COLOR::color c)
{
    uint64_t out = board.piecesBitSets[static_cast<unsigned char>(c)].kingsMask;
    out |= ((NOT_H_COL & out) << 1) | ((NOT_A_COL & out) >> 1);
    out |= out << 8;
    out |= out >> 8;

    return out;
}

constexpr uint64_t getAllFieldsAttackedByKnights(const Board& board, NOTATION::COLOR::color c)
{
    auto fieldBitMask = board.piecesBitSets[static_cast<unsigned char>(c)].knightsMask;

    return (NOT_HG_COL & fieldBitMask) << 10
           | (NOT_HG_COL & fieldBitMask) >> 6
           | (NOT_H_COL & fieldBitMask) >> 15
           | (NOT_H_COL & fieldBitMask) << 17
           | (NOT_AB_COL & fieldBitMask) << 6
           | (NOT_AB_COL & fieldBitMask) >> 10
           | (NOT_A_COL & fieldBitMask) << 15
           | (NOT_A_COL & fieldBitMask) >> 17;
}