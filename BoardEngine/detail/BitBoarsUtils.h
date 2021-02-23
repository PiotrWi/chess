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
