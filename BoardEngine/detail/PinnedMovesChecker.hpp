#pragma once

#include <publicIf/Board.hpp>
#include <publicIf/NotationConversions.hpp>
#include <detail/bitboardslookups.hpp>

uint64_t findPinned(const Board& board, NOTATION::COLOR::color c)
{
    uint64_t pinned = 0ull;
    auto kingBitMask = board.piecesBitSets[static_cast<unsigned char>(c)].kingsMask;

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

    auto ownPieces = board.piecesBitSets[static_cast<unsigned char>(c)].rocksMask
        | board.piecesBitSets[static_cast<unsigned char>(c)].queensMask
        | board.piecesBitSets[static_cast<unsigned char>(c)].kingsMask
        | board.piecesBitSets[static_cast<unsigned char>(c)].knightsMask
        | board.piecesBitSets[static_cast<unsigned char>(c)].pawnsMask
        | board.piecesBitSets[static_cast<unsigned char>(c)].bishopsMask;

    auto OppositeQueenAndRock =
            board.piecesBitSets[static_cast<unsigned char>(c + 1)].queensMask
            | board.piecesBitSets[static_cast<unsigned char>(c + 1)].rocksMask;
    return pinned;
}
