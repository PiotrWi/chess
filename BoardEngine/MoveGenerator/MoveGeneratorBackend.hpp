#pragma once
#include "iostream"

#include "Strategy.hpp"
#include <publicIf/NotationConversions.hpp>
#include <detail/bitboardslookups.hpp>
#include <strings.h>

namespace MoveGenerator
{

template <typename NOTATION::COLOR::color c,
        void (*TVerifyAndAdd)(unsigned char, unsigned char),
        void (*TVerifyAndAddBeating)(unsigned char, unsigned char, unsigned char),
        size_t N,
        const std::pair<unsigned char, unsigned char> TMoves[N]>
class GenerateFixedMoves
{
public:
    static void proccess(unsigned char i)
    {
        constexpr unsigned char TOpositeColor = (static_cast<unsigned char>(c) ^ NOTATION::COLOR::COLOR_MASK);
        const auto row = NotationConversions::getRow(i);
        const auto col = NotationConversions::getColumnNum(i);

        for (auto* diff = TMoves; diff < TMoves + N; ++diff)
        {
            unsigned char targerRow = row + diff->first;
            unsigned char targerCol = col + diff->second;
            if (NotationConversions::isColumnInBoard(targerCol)
                && NotationConversions::isRowInBoard(targerRow))
            {
                unsigned char destination = NotationConversions::getFieldNum(targerRow, targerCol);
                const auto& field = ctx.board->getField(destination);

                if (field == 0)
                {
                    TVerifyAndAdd(i, destination);
                }
                else if ((field & NOTATION::COLOR::COLOR_MASK) == TOpositeColor)
                {
                    TVerifyAndAddBeating(i, destination, field);
                }
            }
        }
    }
};

template <NOTATION::COLOR::color C, template<NOTATION::COLOR::color> typename AddingStrategy>
class GenerateKnightMoves
{
    constexpr static std::pair<unsigned char, unsigned char> knightMoves[] = {
            {1, -2}, {2, -1}, {2, 1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2} };
public:
    static void proccess(unsigned char i)
    {
        GenerateFixedMoves<C,
                AddingStrategy<C>::addKnight,
                AddingStrategy<C>::addKnightWithBeating,
                8u, knightMoves>::proccess(i);
    }
};

template <NOTATION::COLOR::color C>
class GenerateKingMoves
{
    constexpr static const std::pair<unsigned char, unsigned char> kingMoves[] = {
            {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1} };
public:
    static void proccess(unsigned char i)
    {
        GenerateFixedMoves<C,
                StrategyWithAlwaysCheckChecking<C>::addKing,
                StrategyWithAlwaysCheckChecking<C>::addKingWithBeating,
                8u, kingMoves>::proccess(i);
    }
};

template <typename NOTATION::COLOR::color color,
        void (*TVerifyAndAdd)(unsigned char, unsigned char),
        void (*TVerifyAndAddBeating)(unsigned char, unsigned char, unsigned char),
        size_t N,
        const std::pair<unsigned char, unsigned char> TMoves[N]>
class GenerateLineMoves
{
public:
    static void proccess(unsigned char i)
    {
        const auto row = NotationConversions::getRow(i);
        const auto col = NotationConversions::getColumnNum(i);

        for (auto* diff = TMoves; diff < TMoves + N; ++diff)
        {
            for (unsigned char r = row + diff->first, c = col + diff->second;
                 r < 8u and c < 8u;
                 r += diff->first, c+= diff->second)
            {
                auto destination = NotationConversions::getFieldNum(r, c);
                if (ctx.board->getField(destination) != 0)
                {
                    if (static_cast<unsigned char>(ctx.pieceColor+1) ==
                        (ctx.board->getField(destination) & NOTATION::COLOR::COLOR_MASK))
                    {
                        TVerifyAndAddBeating(i, destination, ctx.board->getField(destination));
                    }
                    break;
                }
                TVerifyAndAdd(i, destination);
            }
        }
    };
};

template <NOTATION::COLOR::color C, template<NOTATION::COLOR::color> typename AddingStrategy>
class GenerateRockMoves
{
    constexpr static std::pair<unsigned char, unsigned char> rockMoves[] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
public:
    static void proccess(unsigned char i)
    {
        GenerateLineMoves<C,
                AddingStrategy<C>::addRock,
                AddingStrategy<C>::addRockWithBeating,
                4u, rockMoves>::proccess(i);
    }
};

template <NOTATION::COLOR::color C, template<NOTATION::COLOR::color> typename AddingStrategy>
class GenerateBishopMoves
{
    constexpr static std::pair<unsigned char, unsigned char> bishopMoves[] = {{-1, -1}, {1, -1}, {-1, 1}, {1, 1}};
public:
    static void proccess(unsigned char i)
    {
        GenerateLineMoves<C,
                AddingStrategy<C>::addBishop,
                AddingStrategy<C>::addBishopWithBeating,
                4u, bishopMoves>::proccess(i);
    }
};

template <NOTATION::COLOR::color C, template<NOTATION::COLOR::color> typename AddingStrategy>
class GenerateQueenMoves
{
    constexpr static std::pair<unsigned char, unsigned char> queenMoves[] =
            {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {1, -1}, {-1, 1}, {1, 1}};
public:
    static void proccess(unsigned char i)
    {
        GenerateLineMoves<C,
                AddingStrategy<C>::addBishop,
                AddingStrategy<C>::addBishopWithBeating,
                8u, queenMoves>::proccess(i);
    }
};

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

template <NOTATION::COLOR::color c>
void dispatchToProperHandler(unsigned char i)
{
    switch (ctx.board->getField(i) ^ static_cast<unsigned char>(c))
    {
        case (NOTATION::PIECES::ROCK):
            GenerateRockMoves<c, StrategyWithAlwaysCheckChecking>::proccess(i);
            return;
        case (NOTATION::PIECES::BISHOP):
            GenerateBishopMoves<c, StrategyWithAlwaysCheckChecking>::proccess(i);
            return;
        case (NOTATION::PIECES::QUEEN):
            GenerateQueenMoves<c, StrategyWithAlwaysCheckChecking>::proccess(i);
            return;
        case (NOTATION::PIECES::KING):
            GenerateKingMoves<c>::proccess(i);
            return;
    }
}

template <NOTATION::COLOR::color c>
void generateWithAllMoveAllowance(unsigned char i)
{
    switch (ctx.board->getField(i) ^ static_cast<unsigned char>(c))
    {
        case (NOTATION::PIECES::ROCK):
            GenerateRockMoves<c, StrategyWithNoChecking>::proccess(i);
            return;
        case (NOTATION::PIECES::BISHOP):
            GenerateBishopMoves<c, StrategyWithNoChecking>::proccess(i);
            return;
        case (NOTATION::PIECES::QUEEN):
            GenerateQueenMoves<c, StrategyWithNoChecking>::proccess(i);
            return;
        case (NOTATION::PIECES::KING):
            GenerateKingMoves<c>::proccess(i);
            return;
    }
}

namespace NoLookup
{

template <NOTATION::COLOR::color c>
void evaluateForCheckedPosition()
{
    evaluateForPawns<StrategyWithAlwaysCheckChecking<c>, c>(
            ctx.board->piecesBitSets[static_cast<unsigned char>(c)].pawnsMask);
    evaluateKnights<StrategyWithAlwaysCheckChecking<c>, c>(
            ctx.board->piecesBitSets[static_cast<unsigned char>(c)].knightsMask);
    for (unsigned char i = 0u; i < 64u; ++i)
    {
        dispatchToProperHandler<c>(i);
    }
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

    for (unsigned char i = 0u; i < 64u; ++i)
    {
        if (not ((1lu << i) & pinnedMask) )
        {
            generateWithAllMoveAllowance<c>(i);
        }
        else
        {
            dispatchToProperHandler<c>(i);
        }
    }
}

}  // Namespace NoLookup

}  // namespace MoveGenerator