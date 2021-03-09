#pragma once

#include <publicIf/Board.hpp>
#include <publicIf/NotationConversions.hpp>
#include <detail/bitboardslookups.hpp>
#include <detail/BitBoardsUtils.h>

static uint64_t PinnedRegister[64];

static uint64_t findPinned(const Board& board, NOTATION::COLOR::color c, unsigned char fieldPosition)
{
    uint64_t pinned = 0ull;

    auto allPieces = getAllOccupiedFields(board);
    auto ownPieces = getAllOccupiedPerColor(board, c);

    auto OppositeQueenAndRock =
        board.piecesBitSets[static_cast<unsigned char>(c + 1)].queensMask
        | board.piecesBitSets[static_cast<unsigned char>(c + 1)].rocksMask;

    const auto& lookup = bitBoardLookup[fieldPosition];

    {
        auto northRay = lookup.topRay & allPieces;
        auto northOpponentRocks = northRay & OppositeQueenAndRock;
        if (northOpponentRocks != 0)
        {
            auto rockPosition = __builtin_ffsll(northOpponentRocks) - 1;
            auto rockRay = bitBoardLookup[rockPosition].bottomRay;
            auto allPinedPieces = northRay & rockRay;
            if (__builtin_popcountll(allPinedPieces) == 1) {
                pinned |= allPinedPieces;
                auto pinnedPiecePosition = __builtin_ffsll(allPinedPieces) - 1;
                PinnedRegister[pinnedPiecePosition] =
                        (lookup.topRay & bitBoardLookup[rockPosition].bottomRay) | (1ull << rockPosition);
            }
        }
    }

    {
        auto rightRay = lookup.rightRay & allPieces;
        auto rightOpponentRocks = rightRay & OppositeQueenAndRock;
        if (rightOpponentRocks != 0)
        {
            auto rockPosition = __builtin_ffsll(rightOpponentRocks) - 1;
            auto rockRay = bitBoardLookup[rockPosition].leftRay;
            auto allPinedPieces = rightRay & rockRay;
            if (__builtin_popcountll(allPinedPieces) == 1) {
                pinned |= allPinedPieces;
                auto pinnedPiecePosition = __builtin_ffsll(allPinedPieces) - 1;
                PinnedRegister[pinnedPiecePosition] =
                        (lookup.rightRay & bitBoardLookup[rockPosition].leftRay) | (1ull << rockPosition);
            }
        }
    }

    {
        auto leftRay = lookup.leftRay & allPieces;
        auto leftOpponentRocks = leftRay & OppositeQueenAndRock;
        if (leftOpponentRocks != 0)
        {
            auto rockPosition = (63 - __builtin_clzll(leftOpponentRocks));
            auto rockRay = bitBoardLookup[rockPosition].rightRay;
            auto allPinedPieces = leftRay & rockRay;
            if (__builtin_popcountll(allPinedPieces) == 1) {
                pinned |= allPinedPieces;
                auto pinnedPiecePosition = __builtin_ffsll(allPinedPieces) - 1;
                PinnedRegister[pinnedPiecePosition] =
                        (lookup.leftRay & bitBoardLookup[rockPosition].rightRay) | (1ull << rockPosition);
            }
        }
    }

    {
        auto bottomRay = lookup.bottomRay & allPieces;
        auto bottomOpponentRocks = bottomRay & OppositeQueenAndRock;
        if (bottomOpponentRocks != 0)
        {
            auto rockPosition = (63 - __builtin_clzll(bottomOpponentRocks));
            auto rockRay = bitBoardLookup[rockPosition].topRay;
            auto allPinedPieces = bottomRay & rockRay;
            if (__builtin_popcountll(allPinedPieces) == 1) {
                pinned |= allPinedPieces;
                auto pinnedPiecePosition = __builtin_ffsll(allPinedPieces) - 1;
                PinnedRegister[pinnedPiecePosition] =
                        (lookup.bottomRay & bitBoardLookup[rockPosition].topRay) | (1ull << rockPosition);
            }
        }
    }

    auto OppositeQueenAndBishop =
            board.piecesBitSets[static_cast<unsigned char>(c + 1)].queensMask
            | board.piecesBitSets[static_cast<unsigned char>(c + 1)].bishopsMask;

    {
        auto topLeftRay = lookup.topLeft & allPieces;
        auto topLeftOpponentBishops = topLeftRay & OppositeQueenAndBishop;
        if (topLeftOpponentBishops != 0) {
            auto bishopPosition = __builtin_ffsll(topLeftOpponentBishops) - 1;
            auto bishopRay = bitBoardLookup[bishopPosition].bottomRight;
            auto allPinedPieces = topLeftRay & bishopRay;
            if (__builtin_popcountll(allPinedPieces) == 1) {
                pinned |= allPinedPieces;
                auto pinnedPiecePosition = __builtin_ffsll(allPinedPieces) - 1;
                PinnedRegister[pinnedPiecePosition] =
                        (lookup.topLeft & bitBoardLookup[bishopPosition].bottomRight) | (1ull << bishopPosition);
            }
        }
    }

    {
        auto topRightRay = lookup.topRight & allPieces;
        auto topRightOpponentBishops = topRightRay & OppositeQueenAndBishop;
        if (topRightOpponentBishops != 0)
        {
            auto bishopPosition = __builtin_ffsll(topRightOpponentBishops) - 1;
            auto bishopRay = bitBoardLookup[bishopPosition].bottomLeft;
            auto allPinedPieces = topRightRay & bishopRay;
            if (__builtin_popcountll(allPinedPieces) == 1) {
                pinned |= allPinedPieces;
                auto pinnedPiecePosition = __builtin_ffsll(allPinedPieces) - 1;
                PinnedRegister[pinnedPiecePosition] =
                        (lookup.topRight & bitBoardLookup[bishopPosition].bottomLeft) | (1ull << bishopPosition);
            }
        }
    }

    {
        auto bottomLeftRay = lookup.bottomLeft & allPieces;
        auto bottomLeftOpponentBishops = bottomLeftRay & OppositeQueenAndBishop;
        if (bottomLeftOpponentBishops != 0)
        {
            auto bishopPosition = (63 - __builtin_clzll(bottomLeftOpponentBishops));
            auto bishopRay = bitBoardLookup[bishopPosition].topRight;
            auto allPinedPieces = bottomLeftRay & bishopRay;
            if (__builtin_popcountll(allPinedPieces) == 1) {
                pinned |= allPinedPieces;
                auto pinnedPiecePosition = __builtin_ffsll(allPinedPieces) - 1;
                PinnedRegister[pinnedPiecePosition] =
                        (lookup.bottomLeft & bitBoardLookup[bishopPosition].topRight) | (1ull << bishopPosition);
            }
        }
    }

    {
        auto bottomRightRay = lookup.bottomRight & allPieces;
        auto bottomRightOpponentBishops = bottomRightRay & OppositeQueenAndBishop;
        if (bottomRightOpponentBishops != 0)
        {
            auto bishopPosition = (63 - __builtin_clzll(bottomRightOpponentBishops));
            auto bishopRay = bitBoardLookup[bishopPosition].topLeft;
            auto allPinedPieces = bottomRightRay & bishopRay;
            if (__builtin_popcountll(allPinedPieces) == 1) {
                pinned |= allPinedPieces;
                auto pinnedPiecePosition = __builtin_ffsll(allPinedPieces) - 1;
                PinnedRegister[pinnedPiecePosition] =
                        (lookup.bottomRight & bitBoardLookup[bishopPosition].topRight) | (1ull << bishopPosition);
            }
        }
    }

    return pinned & ownPieces;
}
