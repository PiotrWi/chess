#pragma once
#include "iostream"

#include "Strategy.hpp"
#include "MoveGeneratorContext.hpp"
#include <publicIf/NotationConversions.hpp>
#include <detail/bitboardslookups.hpp>
#include <strings.h>

namespace MoveGenerator
{

template <typename TMoveAddingStrategy, NOTATION::COLOR::color c>
void evaluateForPawns(uint64_t pawnsBitMask)
{
    constexpr uint64_t LINE_4 = 0x00'00'00'00'FF'00'00'00ull;
    constexpr uint64_t LINE_8 = 0xFF'00'00'00'00'00'00'00ull;
    constexpr uint64_t LINE_5 = 0x00'00'00'FF'00'00'00'00ull;
    constexpr uint64_t LINE_1 = 0x00'00'00'00'00'00'00'FFull;

    while (pawnsBitMask)
    {
        auto pawnIndex = 63 - __builtin_clzll(pawnsBitMask);
        auto pawnBitMask = 1ull << pawnIndex;

        if (c == NOTATION::COLOR::color::white)
        {
            //standard moves
            auto nextField = pawnBitMask << 8;
            if (LINE_8 & nextField & ~ctx.allPieces)
            {
                TMoveAddingStrategy::addAndPromote(pawnIndex, pawnIndex + 8);
            } else if (nextField & ~ctx.allPieces)
            {
                TMoveAddingStrategy::addPawn(pawnIndex, pawnIndex + 8);
                nextField <<= 8;
                if (nextField & LINE_4 & ~ctx.allPieces)
                {
                    TMoveAddingStrategy::addPawn(pawnIndex, pawnIndex + 16);
                }
            }
            auto beatingCandidate = ((NOT_H_COL & pawnBitMask) << 9)  & ctx.opponentPieces;
            if (beatingCandidate)
            {
                if (beatingCandidate & LINE_8)
                {
                    TMoveAddingStrategy::addAndPromoteWithBeating(pawnIndex,
                                                  pawnIndex + 9,
                                                  ctx.board->getFieldForNonEmpty(pawnIndex + 9, c+1));
                }
                else
                {
                    TMoveAddingStrategy::addPawnWithBeating(pawnIndex,
                                                pawnIndex + 9,
                                                ctx.board->getFieldForNonEmpty(pawnIndex + 9, c + 1));
                }
            }
            beatingCandidate = ((NOT_A_COL & pawnBitMask) << 7)  & ctx.opponentPieces;
            if (beatingCandidate)
            {
                if (beatingCandidate & LINE_8)
                {
                    TMoveAddingStrategy::addAndPromoteWithBeating(pawnIndex,
                                                      pawnIndex + 7,
                                                      ctx.board->getFieldForNonEmpty(pawnIndex + 7, c+1));
                }
                else
                {
                    TMoveAddingStrategy::addPawnWithBeating(pawnIndex,
                                                    pawnIndex + 7,
                                                    ctx.board->getFieldForNonEmpty(pawnIndex + 7, c+1));
                }
            }
        }
        else
        {
            auto nextField = pawnBitMask >> 8;
            if (LINE_1 & nextField & ~ctx.allPieces)
            {
                TMoveAddingStrategy::addAndPromote(pawnIndex, pawnIndex - 8);
            } else if (nextField & ~ctx.allPieces)
            {
                StrategyWithAlwaysCheckChecking<c>::addPawn(pawnIndex, pawnIndex - 8);
                nextField >>= 8;
                if (nextField & LINE_5 & ~ctx.allPieces)
                {
                    TMoveAddingStrategy::addPawn(pawnIndex, pawnIndex - 16);
                }
            }
            auto beatingCandidate = ((NOT_H_COL & pawnBitMask) >> 7)  & ctx.opponentPieces;
            if (beatingCandidate)
            {
                if (beatingCandidate & LINE_8)
                {
                    TMoveAddingStrategy::addAndPromoteWithBeating(pawnIndex,
                                                          pawnIndex -7,
                                                          ctx.board->getFieldForNonEmpty(pawnIndex - 7, c+1));
                }
                else
                {
                    TMoveAddingStrategy::addPawnWithBeating(pawnIndex,
                                                        pawnIndex -7,
                                                        ctx.board->getFieldForNonEmpty(pawnIndex - 7, c+1));


                }
            }
            beatingCandidate = ((NOT_A_COL & pawnBitMask) >> 9) & ctx.opponentPieces;
            if (beatingCandidate)
            {
                if (beatingCandidate & LINE_8)
                {
                    TMoveAddingStrategy::addAndPromoteWithBeating(pawnIndex,
                                                          pawnIndex - 9,
                                                          ctx.board->getFieldForNonEmpty(pawnIndex - 9, c+1));
                }
                else
                {
                    TMoveAddingStrategy::addPawnWithBeating(pawnIndex,
                                                        pawnIndex - 9,
                                                        ctx.board->getFieldForNonEmpty(pawnIndex - 9, c+1));
                }
            }
        }
        pawnsBitMask ^= pawnBitMask;
    }
}

template <typename TMoveAddingStrategy, NOTATION::COLOR::color c>
void evaluateKnights(uint64_t knightsBitMask)
{
    while (knightsBitMask)
    {
        auto knightIndex = 63 - __builtin_clzll(knightsBitMask);
        auto knightBitMask = 1ull << knightIndex;

        auto quietMoves = bitBoardLookup[knightIndex].knightsMovePossibilities & ~ctx.allPieces;
        while (quietMoves)
        {
            auto targetSquare = 63 - __builtin_clzll(quietMoves);
            TMoveAddingStrategy::addKnight(knightIndex, targetSquare);
            quietMoves ^= (1ull << targetSquare);
        }
        auto beatings = bitBoardLookup[knightIndex].knightsMovePossibilities & ctx.opponentPieces;
        while (beatings)
        {
            auto targetSquare = 63 - __builtin_clzll(beatings);
            TMoveAddingStrategy::addKnightWithBeating(knightIndex,
                                                      targetSquare,
                                                      ctx.board->getFieldForNonEmpty(targetSquare, c+1));
            beatings ^= (1ull << targetSquare);
        }
        knightsBitMask ^= knightBitMask;
    }
};

template <typename TMoveAddingStrategy, NOTATION::COLOR::color c>
void evaluateKing(uint64_t kingBitMask)
{
    auto kingIndex = 63 - __builtin_clzll(kingBitMask);

    auto quietMoves = bitBoardLookup[kingIndex].kingMovePossibilities & ~ctx.allPieces;
    while (quietMoves)
    {
        auto targetSquare = 63 - __builtin_clzll(quietMoves);
        TMoveAddingStrategy::addKing(kingIndex, targetSquare);
        quietMoves ^= (1ull << targetSquare);
    }
    auto beatings = bitBoardLookup[kingIndex].kingMovePossibilities & ctx.opponentPieces;
    while (beatings)
    {
        auto targetSquare = 63 - __builtin_clzll(beatings);
        TMoveAddingStrategy::addKingWithBeating(kingIndex,
                                                  targetSquare,
                                                  ctx.board->getFieldForNonEmpty(targetSquare, c+1));
        beatings ^= (1ull << targetSquare);
    }
};

template <typename TMoveAddingStrategy, NOTATION::COLOR::color c>
void evaluateDiagonal(uint64_t diagonalBitMask)
{
    while (diagonalBitMask)
    {
        auto piecePosition = 63 - __builtin_clzll(diagonalBitMask);
        const auto& lookup = bitBoardLookup[piecePosition];

        uint64_t quietMoves = 0ull;
        uint64_t blockers = 0ull;

        {
            auto topLeftPieces = lookup.topLeft & ctx.allPieces;
            if (topLeftPieces)
            {
                auto topLeftBlocker = __builtin_ffsll(topLeftPieces) - 1;
                quietMoves |=(lookup.topLeft ^ (1ull << topLeftBlocker) ^ bitBoardLookup[topLeftBlocker].topLeft);
                blockers |= (1ull << topLeftBlocker);
            }
            else
            {
                quietMoves |= lookup.topLeft;
            }
        }
        {
            auto topRightPieces = lookup.topRight & ctx.allPieces;
            if (topRightPieces)
            {
                auto topRightBlocker = __builtin_ffsll(topRightPieces) - 1;
                quietMoves |=(lookup.topRight ^ (1ull << topRightBlocker) ^ bitBoardLookup[topRightBlocker].topRight);
                blockers |= (1ull << topRightBlocker);
            }
            else
            {
                quietMoves |= lookup.topRight;
            }
        }
        {
            auto bottomLeftPieces = lookup.bottomLeft & ctx.allPieces;
            if (bottomLeftPieces)
            {
                auto bottomLeftBlocker = 63 - __builtin_clzll(bottomLeftPieces);
                quietMoves |=(lookup.bottomLeft ^ (1ull << bottomLeftBlocker) ^ bitBoardLookup[bottomLeftBlocker].bottomLeft);
                blockers |= (1ull << bottomLeftBlocker);
            }
            else
            {
                quietMoves |= lookup.bottomLeft;
            }
        }
        {
            auto bottomRightPieces = lookup.bottomRight & ctx.allPieces;
            if (bottomRightPieces)
            {
                auto bottomRightBlocker = 63 - __builtin_clzll(bottomRightPieces);
                quietMoves |=(lookup.bottomRight ^ (1ull << bottomRightBlocker) ^ bitBoardLookup[bottomRightBlocker].bottomRight);
                blockers |= (1ull << bottomRightBlocker);
            }
            else
            {
                quietMoves |= lookup.bottomRight;
            }
        }

        while (quietMoves)
        {
            auto targetSquare = 63 - __builtin_clzll(quietMoves);
            TMoveAddingStrategy::addBishop(piecePosition, targetSquare);
            quietMoves ^= (1ull << targetSquare);
        }

        auto possibleAttacks = blockers & ctx.opponentPieces;
        while (possibleAttacks)
        {
            auto targetSquare = 63 - __builtin_clzll(possibleAttacks);
            TMoveAddingStrategy::addBishopWithBeating(piecePosition,
                                                      targetSquare,
                                                      ctx.board->getFieldForNonEmpty(targetSquare, c+1));
            possibleAttacks ^= (1ull << targetSquare);
        }

        diagonalBitMask ^= (1ull << piecePosition);
    }
}

template <typename TMoveAddingStrategy, NOTATION::COLOR::color c>
void evaluateLine(uint64_t lineBitMask)
{
    while (lineBitMask)
    {
        auto piecePosition = 63 - __builtin_clzll(lineBitMask);
        const auto& lookup = bitBoardLookup[piecePosition];

        uint64_t quietMoves = 0ull;
        uint64_t blockers = 0ull;

        {
            auto topPieces = lookup.topRay & ctx.allPieces;
            if (topPieces)
            {
                auto topBlocker = __builtin_ffsll(topPieces) - 1;
                quietMoves |=(lookup.topRay ^ (1ull << topBlocker) ^ bitBoardLookup[topBlocker].topRay);
                blockers |= (1ull << topBlocker);
            }
            else
            {
                quietMoves |= lookup.topRay;
            }
        }
        {
            auto rightPieces = lookup.rightRay & ctx.allPieces;
            if (rightPieces)
            {
                auto rightBlocker = __builtin_ffsll(rightPieces) - 1;
                quietMoves |=(lookup.rightRay ^ (1ull << rightBlocker) ^ bitBoardLookup[rightBlocker].rightRay);
                blockers |= (1ull << rightBlocker);
            }
            else
            {
                quietMoves |= lookup.rightRay;
            }
        }
        {
            auto leftPieces = lookup.leftRay & ctx.allPieces;
            if (leftPieces)
            {
                auto leftBlocker = 63 - __builtin_clzll(leftPieces);
                quietMoves |=(lookup.leftRay ^ (1ull << leftBlocker) ^ bitBoardLookup[leftBlocker].leftRay);
                blockers |= (1ull << leftBlocker);
            }
            else
            {
                quietMoves |= lookup.leftRay;
            }
        }
        {
            auto bottomPieces = lookup.bottomRay & ctx.allPieces;
            if (bottomPieces)
            {
                auto bottomBlocker = 63 - __builtin_clzll(bottomPieces);
                quietMoves |=(lookup.bottomRay ^ (1ull << bottomBlocker) ^ bitBoardLookup[bottomBlocker].bottomRay);
                blockers |= (1ull << bottomBlocker);
            }
            else
            {
                quietMoves |= lookup.bottomRay;
            }
        }

        while (quietMoves)
        {
            auto targetSquare = 63 - __builtin_clzll(quietMoves);
            TMoveAddingStrategy::addRock(piecePosition, targetSquare);
            quietMoves ^= (1ull << targetSquare);
        }

        auto possibleAttacks = blockers & ctx.opponentPieces;
        while (possibleAttacks)
        {
            auto targetSquare = 63 - __builtin_clzll(possibleAttacks);
            TMoveAddingStrategy::addRockWithBeating(piecePosition,
                                                      targetSquare,
                                                      ctx.board->getFieldForNonEmpty(targetSquare, c+1));
            possibleAttacks ^= (1ull << targetSquare);
        }

        lineBitMask ^= (1ull << piecePosition);
    }
}

template <typename TMoveAddingStrategy, NOTATION::COLOR::color c>
void evaluateQueen(uint64_t lineBitMask)
{
    while (lineBitMask)
    {
        auto piecePosition = 63 - __builtin_clzll(lineBitMask);
        const auto& lookup = bitBoardLookup[piecePosition];

        uint64_t quietMoves = 0ull;
        uint64_t blockers = 0ull;
        {
            auto topLeftPieces = lookup.topLeft & ctx.allPieces;
            if (topLeftPieces)
            {
                auto topLeftBlocker = __builtin_ffsll(topLeftPieces) - 1;
                quietMoves |=(lookup.topLeft ^ (1ull << topLeftBlocker) ^ bitBoardLookup[topLeftBlocker].topLeft);
                blockers |= (1ull << topLeftBlocker);
            }
            else
            {
                quietMoves |= lookup.topLeft;
            }
        }
        {
            auto topRightPieces = lookup.topRight & ctx.allPieces;
            if (topRightPieces)
            {
                auto topRightBlocker = __builtin_ffsll(topRightPieces) - 1;
                quietMoves |=(lookup.topRight ^ (1ull << topRightBlocker) ^ bitBoardLookup[topRightBlocker].topRight);
                blockers |= (1ull << topRightBlocker);
            }
            else
            {
                quietMoves |= lookup.topRight;
            }
        }
        {
            auto bottomLeftPieces = lookup.bottomLeft & ctx.allPieces;
            if (bottomLeftPieces)
            {
                auto bottomLeftBlocker = 63 - __builtin_clzll(bottomLeftPieces);
                quietMoves |=(lookup.bottomLeft ^ (1ull << bottomLeftBlocker) ^ bitBoardLookup[bottomLeftBlocker].bottomLeft);
                blockers |= (1ull << bottomLeftBlocker);
            }
            else
            {
                quietMoves |= lookup.bottomLeft;
            }
        }
        {
            auto bottomRightPieces = lookup.bottomRight & ctx.allPieces;
            if (bottomRightPieces)
            {
                auto bottomRightBlocker = 63 - __builtin_clzll(bottomRightPieces);
                quietMoves |=(lookup.bottomRight ^ (1ull << bottomRightBlocker) ^ bitBoardLookup[bottomRightBlocker].bottomRight);
                blockers |= (1ull << bottomRightBlocker);
            }
            else
            {
                quietMoves |= lookup.bottomRight;
            }
        }

        {
            auto topPieces = lookup.topRay & ctx.allPieces;
            if (topPieces)
            {
                auto topBlocker = __builtin_ffsll(topPieces) - 1;
                quietMoves |=(lookup.topRay ^ (1ull << topBlocker) ^ bitBoardLookup[topBlocker].topRay);
                blockers |= (1ull << topBlocker);
            }
            else
            {
                quietMoves |= lookup.topRay;
            }
        }
        {
            auto rightPieces = lookup.rightRay & ctx.allPieces;
            if (rightPieces)
            {
                auto rightBlocker = __builtin_ffsll(rightPieces) - 1;
                quietMoves |=(lookup.rightRay ^ (1ull << rightBlocker) ^ bitBoardLookup[rightBlocker].rightRay);
                blockers |= (1ull << rightBlocker);
            }
            else
            {
                quietMoves |= lookup.rightRay;
            }
        }
        {
            auto leftPieces = lookup.leftRay & ctx.allPieces;
            if (leftPieces)
            {
                auto leftBlocker = 63 - __builtin_clzll(leftPieces);
                quietMoves |=(lookup.leftRay ^ (1ull << leftBlocker) ^ bitBoardLookup[leftBlocker].leftRay);
                blockers |= (1ull << leftBlocker);
            }
            else
            {
                quietMoves |= lookup.leftRay;
            }
        }
        {
            auto bottomPieces = lookup.bottomRay & ctx.allPieces;
            if (bottomPieces)
            {
                auto bottomBlocker = 63 - __builtin_clzll(bottomPieces);
                quietMoves |=(lookup.bottomRay ^ (1ull << bottomBlocker) ^ bitBoardLookup[bottomBlocker].bottomRay);
                blockers |= (1ull << bottomBlocker);
            }
            else
            {
                quietMoves |= lookup.bottomRay;
            }
        }

        while (quietMoves)
        {
            auto targetSquare = 63 - __builtin_clzll(quietMoves);
            TMoveAddingStrategy::addQueen(piecePosition, targetSquare);
            quietMoves ^= (1ull << targetSquare);
        }

        auto possibleAttacks = blockers & ctx.opponentPieces;
        while (possibleAttacks)
        {
            auto targetSquare = 63 - __builtin_clzll(possibleAttacks);
            TMoveAddingStrategy::addQueenWithBeating(piecePosition,
                                                      targetSquare,
                                                      ctx.board->getFieldForNonEmpty(targetSquare, c+1));
            possibleAttacks ^= (1ull << targetSquare);
        }

        lineBitMask ^= (1ull << piecePosition);
    }
}

namespace NoLookup
{

template <NOTATION::COLOR::color c>
void evaluateForCheckedPosition()
{
    evaluateKnights<StrategyWithAlwaysCheckChecking<c>, c>(
            ctx.board->piecesBitSets[static_cast<unsigned char>(c)].knightsMask);
    evaluateForPawns<StrategyWithAlwaysCheckChecking<c>, c>(
            ctx.board->piecesBitSets[static_cast<unsigned char>(c)].pawnsMask);
    evaluateDiagonal<StrategyWithAlwaysCheckChecking<c>, c>(
            ctx.board->piecesBitSets[static_cast<unsigned char>(c)].bishopsMask);
    evaluateLine<StrategyWithAlwaysCheckChecking<c>, c>(
            ctx.board->piecesBitSets[static_cast<unsigned char>(c)].rocksMask);
    evaluateQueen<StrategyWithAlwaysCheckChecking<c>, c>(
            ctx.board->piecesBitSets[static_cast<unsigned char>(c)].queensMask);
    evaluateKing<StrategyWithAlwaysCheckChecking<c>, c>(
            ctx.board->piecesBitSets[static_cast<unsigned char>(c)].kingsMask);
}

template <NOTATION::COLOR::color c>
void evaluateNotCheckedPostions(uint64_t pinnedMask)
{
    evaluateForPawns<StrategyWithNoChecking<c>, c>(
            ~pinnedMask & ctx.board->piecesBitSets[static_cast<unsigned char>(c)].pawnsMask);
    evaluateForPawns<StrategyWithAlwaysCheckChecking<c>, c>(
            pinnedMask & ctx.board->piecesBitSets[static_cast<unsigned char>(c)].pawnsMask);

    evaluateKnights<StrategyWithNoChecking<c>, c>(
            ~pinnedMask & ctx.board->piecesBitSets[static_cast<unsigned char>(c)].knightsMask);
    evaluateKnights<StrategyWithAlwaysCheckChecking<c>, c>(
            pinnedMask & ctx.board->piecesBitSets[static_cast<unsigned char>(c)].knightsMask);

    evaluateDiagonal<StrategyWithNoChecking<c>, c>(
            ~pinnedMask & ctx.board->piecesBitSets[static_cast<unsigned char>(c)].bishopsMask);
    evaluateDiagonal<StrategyWithAlwaysCheckChecking<c>, c>(
            pinnedMask & ctx.board->piecesBitSets[static_cast<unsigned char>(c)].bishopsMask);

    evaluateLine<StrategyWithNoChecking<c>, c>(
            ~pinnedMask & ctx.board->piecesBitSets[static_cast<unsigned char>(c)].rocksMask);
    evaluateLine<StrategyWithAlwaysCheckChecking<c>, c>(
            pinnedMask & ctx.board->piecesBitSets[static_cast<unsigned char>(c)].rocksMask);

    evaluateQueen<StrategyWithNoChecking<c>, c>(
            ~pinnedMask & ctx.board->piecesBitSets[static_cast<unsigned char>(c)].queensMask);
    evaluateQueen<StrategyWithAlwaysCheckChecking<c>, c>(
            pinnedMask & ctx.board->piecesBitSets[static_cast<unsigned char>(c)].queensMask);

    evaluateKing<StrategyWithAlwaysCheckChecking<c>, c>(
            ctx.board->piecesBitSets[static_cast<unsigned char>(c)].kingsMask);
}

}  // Namespace NoLookup

}  // namespace MoveGenerator
