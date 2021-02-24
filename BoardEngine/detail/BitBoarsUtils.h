#pragma once

#include <publicIf/Board.hpp>

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

