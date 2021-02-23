#pragma once

#include <publicIf/Board.hpp>
#include <publicIf/NotationConversions.hpp>
#include <detail/bitboardslookups.hpp>

uint64_t findPinned(const Board& board, NOTATION::COLOR::color c, unsigned char fieldPosition)
{
    uint64_t pinned = 0ull;

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

    const auto& lookup = bitBoardLookup[fieldPosition];

    { // north
        auto northRay = lookup.topRay & allPieces;
        auto northOpponentRocks = northRay & OppositeQueenAndRock;
        if (northOpponentRocks != 0) {
            auto rockPosition = __builtin_ffsll(northOpponentRocks) - 1;
            auto rockRay = bitBoardLookup[rockPosition].bottomRay;
            auto allPinedPieces = northRay & rockRay;
            if (__builtin_popcountll(allPinedPieces) == 1) {
                pinned |= allPinedPieces;
            }
        }
    }

    { // east
        auto rightRay = lookup.rightRay & allPieces;
        auto rightOpponentRocks = rightRay & OppositeQueenAndRock;
        if (rightOpponentRocks != 0)
        {
            auto rockPosition = __builtin_ffsll(rightOpponentRocks) - 1;
            auto rockRay = bitBoardLookup[rockPosition].leftRay;
            auto allPinedPieces = rightRay & rockRay;
            if (__builtin_popcountll(allPinedPieces) == 1) {
                pinned |= allPinedPieces;
            }
        }
    }

    { // west
        auto leftRay = lookup.leftRay & allPieces;
        auto leftOpponentRocks = leftRay & OppositeQueenAndRock;
        if (leftOpponentRocks != 0)
        {
            auto rockPosition = (63 - __builtin_clzll(leftOpponentRocks));
            auto rockRay = bitBoardLookup[rockPosition].rightRay;
            auto allPinedPieces = leftRay & rockRay;
            if (__builtin_popcountll(allPinedPieces) == 1) {
                pinned |= allPinedPieces;
            }
        }
    }

    { // south
        auto bottomRay = lookup.bottomRay & allPieces;
        auto bottomOpponentRocks = bottomRay & OppositeQueenAndRock;
        if (bottomOpponentRocks != 0)
        {
            auto rockPosition = (63 - __builtin_clzll(bottomOpponentRocks));
            auto rockRay = bitBoardLookup[rockPosition].topRay;
            auto allPinedPieces = bottomRay & rockRay;
            if (__builtin_popcountll(allPinedPieces) == 1) {
                pinned |= allPinedPieces;
            }
        }
    }

    auto OppositeQueenAndBishop =
            board.piecesBitSets[static_cast<unsigned char>(c + 1)].queensMask
            | board.piecesBitSets[static_cast<unsigned char>(c + 1)].bishopsMask;

    { // top left
        auto topLeftRay = lookup.topLeft & allPieces;
        auto topLeftOpponentBishops = topLeftRay & OppositeQueenAndBishop;
        if (topLeftOpponentBishops != 0) {
            auto bishopPosition = __builtin_ffsll(topLeftOpponentBishops) - 1;
            auto bishopRay = bitBoardLookup[bishopPosition].bottomRight;
            auto allPinedPieces = topLeftRay & bishopRay;
            if (__builtin_popcountll(allPinedPieces) == 1) {
                pinned |= allPinedPieces;
            }
        }
    }

    { // top right
        auto topRightRay = lookup.topRight & allPieces;
        auto topRightOpponentBishops = topRightRay & OppositeQueenAndBishop;
        if (topRightOpponentBishops != 0)
        {
            auto bishopPosition = __builtin_ffsll(topRightOpponentBishops) - 1;
            auto bishopRay = bitBoardLookup[bishopPosition].bottomLeft;
            auto allPinedPieces = topRightRay & bishopRay;
            if (__builtin_popcountll(allPinedPieces) == 1) {
                pinned |= allPinedPieces;
            }
        }
    }

    { // bottom left
        auto bottomLeftRay = lookup.bottomLeft & allPieces;
        auto bottomLeftOpponentBishops = bottomLeftRay & OppositeQueenAndBishop;
        if (bottomLeftOpponentBishops != 0)
        {
            auto bishopPosition = (63 - __builtin_clzll(bottomLeftOpponentBishops));
            auto bishopRay = bitBoardLookup[bishopPosition].topRight;
            auto allPinedPieces = bottomLeftRay & bishopRay;
            if (__builtin_popcountll(allPinedPieces) == 1) {
                pinned |= allPinedPieces;
            }
        }
    }

    { // bottom right
        auto bottomRightRay = lookup.bottomRight & allPieces;
        auto bottomRightOpponentBishops = bottomRightRay & OppositeQueenAndBishop;
        if (bottomRightOpponentBishops != 0)
        {
            auto bishopPosition = (63 - __builtin_clzll(bottomRightOpponentBishops));
            auto bishopRay = bitBoardLookup[bishopPosition].topLeft;
            auto allPinedPieces = bottomRightRay & bishopRay;
            if (__builtin_popcountll(allPinedPieces) == 1) {
                pinned |= allPinedPieces;
            }
        }
    }

    return pinned & ownPieces;
}
