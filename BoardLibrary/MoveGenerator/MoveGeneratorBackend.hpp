#pragma once
#include "iostream"

#include "Strategy.hpp"
#include "MoveGeneratorContext.hpp"
#include <publicIf/NotationConversions.hpp>
#include <detail/bitboardslookups.hpp>
#include <strings.h>
#include <detail/PinnedMovesChecker.hpp>

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

        if constexpr (c == NOTATION::COLOR::color::white)
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
                if (beatingCandidate & LINE_1)
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
                if (beatingCandidate & LINE_1)
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

        uint64_t allAttacks = bishopMagicBb.getAttacksFor(piecePosition, ctx.allPieces);

        uint64_t quietMoves = allAttacks &(~ctx.allPieces);
        while (quietMoves)
        {
            auto targetSquare = 63 - __builtin_clzll(quietMoves);
            TMoveAddingStrategy::addBishop(piecePosition, targetSquare);
            quietMoves ^= (1ull << targetSquare);
        }

        auto possibleAttacks = allAttacks & ctx.opponentPieces;
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

        uint64_t allAttacks = rockMagicBb.getAttacksFor(piecePosition, ctx.allPieces);

        uint64_t quietMoves = allAttacks &(~ctx.allPieces);
        while (quietMoves)
        {
            auto targetSquare = 63 - __builtin_clzll(quietMoves);
            TMoveAddingStrategy::addRock(piecePosition, targetSquare);
            quietMoves ^= (1ull << targetSquare);
        }

        auto possibleAttacks = allAttacks & ctx.opponentPieces;
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

        uint64_t allAttacks = rockMagicBb.getAttacksFor(piecePosition, ctx.allPieces)
                | bishopMagicBb.getAttacksFor(piecePosition, ctx.allPieces);


        uint64_t quietMoves = allAttacks &(~ctx.allPieces);
        while (quietMoves)
        {
            auto targetSquare = 63 - __builtin_clzll(quietMoves);
            TMoveAddingStrategy::addQueen(piecePosition, targetSquare);
            quietMoves ^= (1ull << targetSquare);
        }

        auto possibleAttacks = allAttacks & ctx.opponentPieces;
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

namespace PinnedNotChecked
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
            auto nextField = (pawnBitMask << 8) & PinnedRegister[pawnIndex];
            if (LINE_8 & nextField & ~ctx.allPieces)
            {
                TMoveAddingStrategy::addAndPromote(pawnIndex, pawnIndex + 8);
            } else if (nextField & ~ctx.allPieces)
            {
                TMoveAddingStrategy::addPawn(pawnIndex, pawnIndex + 8);
                nextField = (nextField << 8) & PinnedRegister[pawnIndex];
                if (nextField & LINE_4 & ~ctx.allPieces)
                {
                    TMoveAddingStrategy::addPawn(pawnIndex, pawnIndex + 16);
                }
            }
            auto beatingCandidate = ((NOT_H_COL & pawnBitMask) << 9) & ctx.opponentPieces & PinnedRegister[pawnIndex];
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
            beatingCandidate = ((NOT_A_COL & pawnBitMask) << 7)  & ctx.opponentPieces & PinnedRegister[pawnIndex];
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
            auto nextField = pawnBitMask >> 8 & PinnedRegister[pawnIndex];
            if (LINE_1 & nextField & ~ctx.allPieces)
            {
                TMoveAddingStrategy::addAndPromote(pawnIndex, pawnIndex - 8);
            } else if (nextField & ~ctx.allPieces)
            {
                StrategyWithAlwaysCheckChecking<c>::addPawn(pawnIndex, pawnIndex - 8);
                nextField = (nextField >> 8) & PinnedRegister[pawnIndex];
                if (nextField & LINE_5 & ~ctx.allPieces)
                {
                    TMoveAddingStrategy::addPawn(pawnIndex, pawnIndex - 16);
                }
            }
            auto beatingCandidate = ((NOT_H_COL & pawnBitMask) >> 7)  & ctx.opponentPieces & PinnedRegister[pawnIndex];;
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
            beatingCandidate = ((NOT_A_COL & pawnBitMask) >> 9) & ctx.opponentPieces & PinnedRegister[pawnIndex];;
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
void evaluateDiagonal(uint64_t diagonalBitMask)
{
    while (diagonalBitMask)
    {
        auto piecePosition = 63 - __builtin_clzll(diagonalBitMask);

        uint64_t allAttacks = bishopMagicBb.getAttacksFor(piecePosition, ctx.allPieces);

        uint64_t quietMoves = allAttacks &(~ctx.allPieces) & PinnedRegister[piecePosition];;
        while (quietMoves)
        {
            auto targetSquare = 63 - __builtin_clzll(quietMoves);
            TMoveAddingStrategy::addBishop(piecePosition, targetSquare);
            quietMoves ^= (1ull << targetSquare);
        }

        auto possibleAttacks = allAttacks & ctx.opponentPieces & PinnedRegister[piecePosition];
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

        uint64_t allAttacks = rockMagicBb.getAttacksFor(piecePosition, ctx.allPieces);

        uint64_t quietMoves = allAttacks &(~ctx.allPieces) & PinnedRegister[piecePosition];
        while (quietMoves)
        {
            auto targetSquare = 63 - __builtin_clzll(quietMoves);
            TMoveAddingStrategy::addRock(piecePosition, targetSquare);
            quietMoves ^= (1ull << targetSquare);
        }

        auto possibleAttacks = allAttacks & ctx.opponentPieces& PinnedRegister[piecePosition];
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
        uint64_t allAttacks = rockMagicBb.getAttacksFor(piecePosition, ctx.allPieces) | bishopMagicBb.getAttacksFor(piecePosition, ctx.allPieces);

        uint64_t quietMoves = allAttacks &(~ctx.allPieces) & PinnedRegister[piecePosition];
        while (quietMoves)
        {
            auto targetSquare = 63 - __builtin_clzll(quietMoves);
            TMoveAddingStrategy::addQueen(piecePosition, targetSquare);
            quietMoves ^= (1ull << targetSquare);
        }

        auto possibleAttacks = allAttacks & ctx.opponentPieces& PinnedRegister[piecePosition];
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

} // namespace PinnedNotChecked

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
void evaluateNotCheckedPostions(Pinnes pinnes)
{
    evaluateForPawns<StrategyWithNoChecking<c>, c>(
            ~pinnes.allPinned & ctx.board->piecesBitSets[static_cast<unsigned char>(c)].pawnsMask);
    PinnedNotChecked::evaluateForPawns<StrategyWithNoChecking<c>, c>(
            (pinnes.allPinned ^ pinnes.horizontallyPinned) & ctx.board->piecesBitSets[static_cast<unsigned char>(c)].pawnsMask);

    evaluateKnights<StrategyWithNoChecking<c>, c>(
            ~pinnes.allPinned & ctx.board->piecesBitSets[static_cast<unsigned char>(c)].knightsMask);

    evaluateDiagonal<StrategyWithNoChecking<c>, c>(
            ~pinnes.allPinned & ctx.board->piecesBitSets[static_cast<unsigned char>(c)].bishopsMask);
    PinnedNotChecked::evaluateDiagonal<StrategyWithNoChecking<c>, c>(
            (pinnes.diagonallyPinnedFromLeftBottom | pinnes.diagonallyPinnedFromLeftTop) & ctx.board->piecesBitSets[static_cast<unsigned char>(c)].bishopsMask);

    evaluateLine<StrategyWithNoChecking<c>, c>(
            ~pinnes.allPinned & ctx.board->piecesBitSets[static_cast<unsigned char>(c)].rocksMask);
    PinnedNotChecked::evaluateLine<StrategyWithNoChecking<c>, c>(
            (pinnes.verticallyPinned | pinnes.horizontallyPinned) & ctx.board->piecesBitSets[static_cast<unsigned char>(c)].rocksMask);

    evaluateQueen<StrategyWithNoChecking<c>, c>(
            ~pinnes.allPinned & ctx.board->piecesBitSets[static_cast<unsigned char>(c)].queensMask);
    PinnedNotChecked::evaluateQueen<StrategyWithNoChecking<c>, c>(
            pinnes.allPinned & ctx.board->piecesBitSets[static_cast<unsigned char>(c)].queensMask);

    evaluateKing<StrategyWithAlwaysCheckChecking<c>, c>(
            ctx.board->piecesBitSets[static_cast<unsigned char>(c)].kingsMask);
}

}  // Namespace NoLookup

}  // namespace MoveGenerator
