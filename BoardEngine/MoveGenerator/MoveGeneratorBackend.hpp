#pragma once

#include "Strategy.hpp"
#include <publicIf/NotationConversions.hpp>

namespace MoveGenerator
{

template <typename TMoveAddingStrategy
        , unsigned char FIRST_LINE
        , unsigned char LINE_BEFORE_PROMOTION
        , signed char ROW_DIFF
        , unsigned char OPOSITE_COLOR>
void generateStandardPawnMoves(unsigned char i)
{
    if ((*ctx.board)[i + ROW_DIFF] == 0)
    {
        auto row = NotationConversions::getRow(i);
        if (row == LINE_BEFORE_PROMOTION)
        {
            TMoveAddingStrategy::addAndPromote(i, i + ROW_DIFF);
        }
        else
        {
            TMoveAddingStrategy::addPawn(i, i + ROW_DIFF);
        }
        if (row == FIRST_LINE and (*ctx.board)[i + 2 * ROW_DIFF] == 0)
        {
            TMoveAddingStrategy::addPawn(i, i + 2 * ROW_DIFF);
        }
    }
    auto col = NotationConversions::getColumnNum(i);
    if (col<7)
    {
        auto destination = i + ROW_DIFF + 1;
        if (((*ctx.board)[destination] & NOTATION::COLOR::COLOR_MASK) == OPOSITE_COLOR)
        {
            auto row = NotationConversions::getRow(i);
            if (row == LINE_BEFORE_PROMOTION)
            {
                TMoveAddingStrategy::addAndPromoteWithBeating(i, destination, (*ctx.board)[destination]);            }
            else
            {
                TMoveAddingStrategy::addPawnWithBeating(i, destination, (*ctx.board)[destination]);
            }
        }
    }
    if (col > 0)
    {
        auto destination = i + ROW_DIFF - 1;
        if (((*ctx.board)[destination] & NOTATION::COLOR::COLOR_MASK) == OPOSITE_COLOR)
        {
            auto row = NotationConversions::getRow(i);
            if (row == LINE_BEFORE_PROMOTION)
            {
                TMoveAddingStrategy::addAndPromoteWithBeating(i, destination, (*ctx.board)[destination]);            }
            else
            {
                TMoveAddingStrategy::addPawnWithBeating(i, destination, (*ctx.board)[destination]);
            }
        }
    }
}


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
                const auto& field = (*ctx.board)[destination];

                if (field == 0)
                {
                    TVerifyAndAdd(i, destination);
                }
                if ((field & NOTATION::COLOR::COLOR_MASK) == TOpositeColor)
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
                if ((*ctx.board)[destination] != 0)
                {
                    if (static_cast<unsigned char>(ctx.pieceColor+1) ==
                        ((*ctx.board)[destination] & NOTATION::COLOR::COLOR_MASK))
                    {
                        TVerifyAndAddBeating(i, destination, (*ctx.board)[destination]);
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
void dispatchToGenerateStandardPawnMoves(unsigned char i);

template<>
void dispatchToGenerateStandardPawnMoves<StrategyWithAlwaysCheckChecking<NOTATION::COLOR::color::white>,
        NOTATION::COLOR::color::white>(unsigned char i)
{
generateStandardPawnMoves<StrategyWithAlwaysCheckChecking<NOTATION::COLOR::color::white>,
1u, 6u,
NOTATION::COORDINATES::ROW_DIFF,
NOTATION::COLOR::BLACK>(i);
}

template<>
void dispatchToGenerateStandardPawnMoves<StrategyWithAlwaysCheckChecking<NOTATION::COLOR::color::black>, NOTATION::COLOR::color::black>(unsigned char i)
{
generateStandardPawnMoves<StrategyWithAlwaysCheckChecking<NOTATION::COLOR::color::black>,
        6u, 1u,
        -NOTATION::COORDINATES::ROW_DIFF,
        NOTATION::COLOR::WHITE>(i);
}

template<>
void dispatchToGenerateStandardPawnMoves<StrategyWithNoChecking<NOTATION::COLOR::color::white>, NOTATION::COLOR::color::white>(unsigned char i)
{
generateStandardPawnMoves<StrategyWithNoChecking<NOTATION::COLOR::color::white>,
        1u, 6u,
        NOTATION::COORDINATES::ROW_DIFF,
        NOTATION::COLOR::BLACK>(i);
}

template<>
void dispatchToGenerateStandardPawnMoves<StrategyWithNoChecking<NOTATION::COLOR::color::black>, NOTATION::COLOR::color::black>(unsigned char i)
{
generateStandardPawnMoves<StrategyWithNoChecking<NOTATION::COLOR::color::black>,
        6u, 1u,
        -NOTATION::COORDINATES::ROW_DIFF,
        NOTATION::COLOR::WHITE>(i);
}

template <NOTATION::COLOR::color c>
void dispatchToProperHandler(unsigned char i)
{
    switch ((*ctx.board)[i] ^ static_cast<unsigned char>(c))
    {
        case (NOTATION::PIECES::PAWN):
            dispatchToGenerateStandardPawnMoves<StrategyWithAlwaysCheckChecking<c>, c>(i);
            return;
        case (NOTATION::PIECES::KNIGHT):
            GenerateKnightMoves<c, StrategyWithAlwaysCheckChecking>::proccess(i);
            return;
        case (NOTATION::PIECE_FEATURES::CAN_ATTACK_ON_LINES):
            GenerateRockMoves<c, StrategyWithAlwaysCheckChecking>::proccess(i);
            return;
        case (NOTATION::PIECE_FEATURES::CAN_ATTACK_ON_DIAGONAL):
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
    switch ((*ctx.board)[i] ^ static_cast<unsigned char>(c))
    {
        case (NOTATION::PIECES::PAWN):
            dispatchToGenerateStandardPawnMoves<StrategyWithNoChecking<c>, c>(i);
            return;
        case (NOTATION::PIECES::KNIGHT):
            GenerateKnightMoves<c, StrategyWithNoChecking>::proccess(i);
            return;
        case (NOTATION::PIECE_FEATURES::CAN_ATTACK_ON_LINES):
            GenerateRockMoves<c, StrategyWithNoChecking>::proccess(i);
            return;
        case (NOTATION::PIECE_FEATURES::CAN_ATTACK_ON_DIAGONAL):
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
    for (unsigned char i = 0u; i < 64u; ++i)
    {
        dispatchToProperHandler<c>(i);
    }
}

template <NOTATION::COLOR::color c>
void evaluateNotCheckedPostions(uint64_t pinnedMask)
{
    uint64_t wasChecked = 0;
    wasChecked |= (1lu << ctx.kingPosition); // King moves are last to evaluate.

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