#pragma once

#include "MoveGeneratorContext.hpp"
#include "Strategy.hpp"

#include <publicIf/NotationConversions.hpp>
#include <detail/CheckChecker.hpp>
#include <FieldLookup/FieldLookup.hpp>

namespace MoveGenerator
{

template <NOTATION::COLOR::color C, const unsigned char i, template<NOTATION::COLOR::color> typename AddingStrategy>
void generateStandardPawnMoves()
{
    constexpr signed char ROW_DIFF = (C == NOTATION::COLOR::color::white)
                                     ? NOTATION::COORDINATES::ROW_DIFF
                                     : -1 * NOTATION::COORDINATES::ROW_DIFF;

    if ((*ctx.board)[i + ROW_DIFF] == 0)
    {
        if (FieldLookup<C, i>::CanPawnPromote)
        {
            AddingStrategy<C>::addAndPromote(i, i + ROW_DIFF);
        }
        else
        {
            AddingStrategy<C>::addPawn(i, i + ROW_DIFF);
        }
        if (FieldLookup<C, i>::CanPawnMove2Fields and (*ctx.board)[i + 2 * ROW_DIFF] == 0)
        {
            AddingStrategy<C>::addPawn(i, i + 2 * ROW_DIFF);
        }
    }
    for (const unsigned char* pos = FieldLookup<C, i>::PawnAttackCandidates;
         pos < FieldLookup<C, i>::PawnAttackCandidates + FieldLookup<C, i>::PawnAttackCandidatesSize
            ; ++pos)
    {
        constexpr unsigned char OPOSITE_COLOR = static_cast<unsigned char>(C) ^ NOTATION::COLOR::COLOR_MASK;
        if (((*ctx.board)[*pos] & NOTATION::COLOR::COLOR_MASK) == OPOSITE_COLOR)
        {
            if (FieldLookup<C, i>::CanPawnPromote)
            {
                AddingStrategy<C>::addAndPromoteWithBeating(i, *pos, (*ctx.board)[*pos]);
            }
            else
            {
                AddingStrategy<C>::addPawnWithBeating(i, *pos, (*ctx.board)[*pos]);
            }
        }
    }
}

template <NOTATION::COLOR::color C, const unsigned char i, template<NOTATION::COLOR::color> typename AddingStrategy>
class GenerateKnightMoves2
{
public:
    static void proccess()
    {
        constexpr unsigned char TOpositeColor = (static_cast<unsigned char>(C) ^ NOTATION::COLOR::COLOR_MASK);

        for (const auto* pos = FieldLookup<C, i>::KnightPossibleMoves;
             pos < FieldLookup<C, i>::KnightPossibleMoves + FieldLookup<C, i>::KnightPossibleMovesSize;
             ++pos)
        {
            const auto& field = (*ctx.board)[*pos];

            if (field == 0)
            {
                AddingStrategy<C>::addKnight(i, *pos);
            }
            if ((field & NOTATION::COLOR::COLOR_MASK) == TOpositeColor)
            {
                AddingStrategy<C>::addKnightWithBeating(i, *pos, field);
            }
        }
    }
};

template <NOTATION::COLOR::color C, const unsigned char i>
class GenerateKingMoves2
{
public:
    static void proccess()
    {
        constexpr unsigned char TOpositeColor = (static_cast<unsigned char>(C) ^ NOTATION::COLOR::COLOR_MASK);

        for (const auto* pos = FieldLookup<C, i>::KingPossibleMoves;
             pos < FieldLookup<C, i>::KingPossibleMoves + FieldLookup<C, i>::KingPossibleMovesSize;
             ++pos)
        {
            const auto& field = (*ctx.board)[*pos];

            if (field == 0)
            {
                StrategyWithAlwaysCheckChecking<C>::addKing(i, *pos);
            }
            if ((field & NOTATION::COLOR::COLOR_MASK) == TOpositeColor)
            {
                StrategyWithAlwaysCheckChecking<C>::addKingWithBeating(i, *pos, field);
            }
        }
    }
};

template <NOTATION::COLOR::color C, const unsigned int i, template<NOTATION::COLOR::color> typename AddingStrategy>
class GenerateRockMoves2
{
public:
    static void proccess()
    {
        constexpr auto TOpositeColor = static_cast<unsigned char>(C+1);

        for (const auto* pos = FieldLookup<C, i>::BottomPossibleMoves;
             pos < FieldLookup<C, i>::BottomPossibleMoves+FieldLookup<C,i>::BottomPossibleMovesSize;
             ++pos)
        {
            if ((*ctx.board)[*pos] != 0)
            {
                if (TOpositeColor ==
                    ((*ctx.board)[*pos] & NOTATION::COLOR::COLOR_MASK))
                {
                    AddingStrategy<C>::addRockWithBeating(i, *pos, (*ctx.board)[*pos]);
                }
                break;
            }
            AddingStrategy<C>::addRock(i, *pos);
        }
        for (const auto* pos = FieldLookup<C, i>::TopPossibleMoves;
             pos < FieldLookup<C, i>::TopPossibleMoves+FieldLookup<C,i>::TopPossibleMovesSize;
             ++pos)
        {
            if ((*ctx.board)[*pos] != 0)
            {
                if (TOpositeColor ==
                    ((*ctx.board)[*pos] & NOTATION::COLOR::COLOR_MASK))
                {
                    AddingStrategy<C>::addRockWithBeating(i, *pos, (*ctx.board)[*pos]);
                }
                break;
            }
            AddingStrategy<C>::addRock(i, *pos);
        }
        for (const auto* pos = FieldLookup<C, i>::LeftPossibleMoves;
             pos < FieldLookup<C, i>::LeftPossibleMoves+FieldLookup<C,i>::LeftPossibleMovesSize;
             ++pos)
        {
            if ((*ctx.board)[*pos] != 0)
            {
                if (TOpositeColor ==
                    ((*ctx.board)[*pos] & NOTATION::COLOR::COLOR_MASK))
                {
                    AddingStrategy<C>::addRockWithBeating(i, *pos, (*ctx.board)[*pos]);
                }
                break;
            }
            AddingStrategy<C>::addRock(i, *pos);
        }
        for (const auto* pos = FieldLookup<C, i>::RightPossibleMoves;
             pos < FieldLookup<C, i>::RightPossibleMoves+FieldLookup<C,i>::RightPossibleMovesSize;
             ++pos)
        {
            if ((*ctx.board)[*pos] != 0)
            {
                if (TOpositeColor ==
                    ((*ctx.board)[*pos] & NOTATION::COLOR::COLOR_MASK))
                {
                    AddingStrategy<C>::addRockWithBeating(i, *pos, (*ctx.board)[*pos]);
                }
                break;
            }
            AddingStrategy<C>::addRock(i, *pos);
        }
    }
};

template <NOTATION::COLOR::color C, const unsigned int i, template<NOTATION::COLOR::color> typename AddingStrategy>
class GenerateBishopMoves2
{
public:
    static void proccess()
    {
        constexpr auto TOpositeColor = static_cast<unsigned char>(C+1);

        constexpr auto blend = FieldLookup<C, i>::BottomLeftPossibleMoves+FieldLookup<C,i>::BottomLeftPossibleMovesSize;
        for (const auto* pos = FieldLookup<C, i>::BottomLeftPossibleMoves;
             pos < blend;
             ++pos)
        {
            if ((*ctx.board)[*pos] != 0)
            {
                if (TOpositeColor ==
                    ((*ctx.board)[*pos] & NOTATION::COLOR::COLOR_MASK))
                {
                    AddingStrategy<C>::addBishopWithBeating(i, *pos, (*ctx.board)[*pos]);
                }
                break;
            }
            AddingStrategy<C>::addBishop(i, *pos);
        }
        constexpr auto tlend = FieldLookup<C, i>::TopLeftPossibleMoves+FieldLookup<C,i>::TopLeftPossibleMovesSize;
        for (const auto* pos = FieldLookup<C, i>::TopLeftPossibleMoves;
             pos < tlend;
             ++pos)
        {
            if ((*ctx.board)[*pos] != 0)
            {
                if (TOpositeColor ==
                    ((*ctx.board)[*pos] & NOTATION::COLOR::COLOR_MASK))
                {
                    AddingStrategy<C>::addBishopWithBeating(i, *pos, (*ctx.board)[*pos]);
                }
                break;
            }
            AddingStrategy<C>::addBishop(i, *pos);
        }
        constexpr auto brend = FieldLookup<C, i>::BottomRightPossibleMoves+FieldLookup<C,i>::BottomRightPossibleMovesSize;
        for (const auto* pos = FieldLookup<C, i>::BottomRightPossibleMoves;
             pos < brend;
             ++pos)
        {
            if ((*ctx.board)[*pos] != 0)
            {
                if (TOpositeColor ==
                    ((*ctx.board)[*pos] & NOTATION::COLOR::COLOR_MASK))
                {
                    AddingStrategy<C>::addBishopWithBeating(i, *pos, (*ctx.board)[*pos]);
                }
                break;
            }
            AddingStrategy<C>::addBishop(i, *pos);
        }
        constexpr auto trend = FieldLookup<C, i>::TopRightPossibleMoves+FieldLookup<C,i>::TopRightPossibleMovesSize;
        for (const auto* pos = FieldLookup<C, i>::TopRightPossibleMoves;
             pos < trend;
             ++pos)
        {
            if ((*ctx.board)[*pos] != 0)
            {
                if (TOpositeColor ==
                    ((*ctx.board)[*pos] & NOTATION::COLOR::COLOR_MASK))
                {
                    AddingStrategy<C>::addBishopWithBeating(i, *pos, (*ctx.board)[*pos]);
                }
                break;
            }
            AddingStrategy<C>::addBishop(i, *pos);
        }
    }
};


template <NOTATION::COLOR::color C, const unsigned int i, template<NOTATION::COLOR::color> typename AddingStrategy>
class GenerateQueenMoves2
{
public:
    static void proccess()
    {
        constexpr auto TOpositeColor = static_cast<unsigned char>(C+1);

        constexpr auto bend = FieldLookup<C, i>::BottomPossibleMoves+FieldLookup<C,i>::BottomPossibleMovesSize;
        for (const auto* pos = FieldLookup<C, i>::BottomPossibleMoves;
             pos < bend;
             ++pos)
        {
            if ((*ctx.board)[*pos] != 0)
            {
                if (TOpositeColor ==
                    ((*ctx.board)[*pos] & NOTATION::COLOR::COLOR_MASK))
                {
                    AddingStrategy<C>::addQueenWithBeating(i, *pos, (*ctx.board)[*pos]);
                }
                break;
            }
            AddingStrategy<C>::addQueen(i, *pos);
        }

        constexpr auto tend = FieldLookup<C, i>::TopPossibleMoves+FieldLookup<C,i>::TopPossibleMovesSize;
        for (const auto* pos = FieldLookup<C, i>::TopPossibleMoves;
             pos < tend;
             ++pos)
        {
            if ((*ctx.board)[*pos] != 0)
            {
                if (TOpositeColor ==
                    ((*ctx.board)[*pos] & NOTATION::COLOR::COLOR_MASK))
                {
                    AddingStrategy<C>::addQueenWithBeating(i, *pos, (*ctx.board)[*pos]);
                }
                break;
            }
            AddingStrategy<C>::addQueen(i, *pos);
        }
        constexpr auto lend = FieldLookup<C, i>::LeftPossibleMoves+FieldLookup<C,i>::LeftPossibleMovesSize;
        for (const auto* pos = FieldLookup<C, i>::LeftPossibleMoves;
             pos < lend;
             ++pos)
        {
            if ((*ctx.board)[*pos] != 0)
            {
                if (TOpositeColor ==
                    ((*ctx.board)[*pos] & NOTATION::COLOR::COLOR_MASK))
                {
                    AddingStrategy<C>::addQueenWithBeating(i, *pos, (*ctx.board)[*pos]);
                }
                break;
            }
            AddingStrategy<C>::addQueen(i, *pos);
        }
        constexpr auto rend = FieldLookup<C, i>::RightPossibleMoves+FieldLookup<C,i>::RightPossibleMovesSize;
        for (const auto* pos = FieldLookup<C, i>::RightPossibleMoves;
             pos < rend;
             ++pos)
        {
            if ((*ctx.board)[*pos] != 0)
            {
                if (TOpositeColor ==
                    ((*ctx.board)[*pos] & NOTATION::COLOR::COLOR_MASK))
                {
                    AddingStrategy<C>::addQueenWithBeating(i, *pos, (*ctx.board)[*pos]);
                }
                break;
            }
            AddingStrategy<C>::addQueen(i, *pos);
        }

        constexpr auto blend = FieldLookup<C, i>::BottomLeftPossibleMoves+FieldLookup<C,i>::BottomLeftPossibleMovesSize;
        for (const auto* pos = FieldLookup<C, i>::BottomLeftPossibleMoves;
             pos < blend;
             ++pos)
        {
            if ((*ctx.board)[*pos] != 0)
            {
                if (TOpositeColor ==
                    ((*ctx.board)[*pos] & NOTATION::COLOR::COLOR_MASK))
                {
                    AddingStrategy<C>::addQueenWithBeating(i, *pos, (*ctx.board)[*pos]);
                }
                break;
            }
            AddingStrategy<C>::addQueen(i, *pos);
        }
        constexpr auto tlend = FieldLookup<C, i>::TopLeftPossibleMoves+FieldLookup<C,i>::TopLeftPossibleMovesSize;
        for (const auto* pos = FieldLookup<C, i>::TopLeftPossibleMoves;
             pos < tlend;
             ++pos)
        {
            if ((*ctx.board)[*pos] != 0)
            {
                if (TOpositeColor ==
                    ((*ctx.board)[*pos] & NOTATION::COLOR::COLOR_MASK))
                {
                    AddingStrategy<C>::addQueenWithBeating(i, *pos, (*ctx.board)[*pos]);
                }
                break;
            }
            AddingStrategy<C>::addQueen(i, *pos);
        }
        constexpr auto brend = FieldLookup<C, i>::BottomRightPossibleMoves+FieldLookup<C,i>::BottomRightPossibleMovesSize;
        for (const auto* pos = FieldLookup<C, i>::BottomRightPossibleMoves;
             pos < brend;
             ++pos)
        {
            if ((*ctx.board)[*pos] != 0)
            {
                if (TOpositeColor ==
                    ((*ctx.board)[*pos] & NOTATION::COLOR::COLOR_MASK))
                {
                    AddingStrategy<C>::addQueenWithBeating(i, *pos, (*ctx.board)[*pos]);
                }
                break;
            }
            AddingStrategy<C>::addQueen(i, *pos);
        }
        constexpr auto trend = FieldLookup<C, i>::TopRightPossibleMoves+FieldLookup<C,i>::TopRightPossibleMovesSize;
        for (const auto* pos = FieldLookup<C, i>::TopRightPossibleMoves;
             pos < trend;
             ++pos)
        {
            if ((*ctx.board)[*pos] != 0)
            {
                if (TOpositeColor ==
                    ((*ctx.board)[*pos] & NOTATION::COLOR::COLOR_MASK))
                {
                    AddingStrategy<C>::addQueenWithBeating(i, *pos, (*ctx.board)[*pos]);
                }
                break;
            }
            AddingStrategy<C>::addQueen(i, *pos);
        }
    }
};

template <NOTATION::COLOR::color c, const unsigned char i>
void dispatchToProperHandler()
{
    switch ((*ctx.board)[i] ^ static_cast<unsigned char>(c))
    {
        case (NOTATION::PIECES::PAWN):
            generateStandardPawnMoves<c, i, StrategyWithAlwaysCheckChecking>();
            return;
        case (NOTATION::PIECES::KNIGHT):
            GenerateKnightMoves2<c, i, StrategyWithAlwaysCheckChecking>::proccess();
            return;
        case (NOTATION::PIECE_FEATURES::CAN_ATTACK_ON_LINES):
            GenerateRockMoves2<c, i, StrategyWithAlwaysCheckChecking>::proccess();
            return;
        case (NOTATION::PIECE_FEATURES::CAN_ATTACK_ON_DIAGONAL):
            GenerateBishopMoves2<c, i, StrategyWithAlwaysCheckChecking>::proccess();
            return;
        case (NOTATION::PIECES::QUEEN):
            GenerateQueenMoves2<c, i, StrategyWithAlwaysCheckChecking>::proccess();
            return;
        case (NOTATION::PIECES::KING):
            GenerateKingMoves2<c, i>::proccess();
            return;
    }
}

template <NOTATION::COLOR::color c, const unsigned char i>
void generateWithAllMoveAllowance()
{
    switch ((*ctx.board)[i] ^ static_cast<unsigned char>(c))
    {
        case (NOTATION::PIECES::PAWN):
            generateStandardPawnMoves<c, i, StrategyWithNoChecking>();
            return;
        case (NOTATION::PIECES::KNIGHT):
            GenerateKnightMoves2<c, i, StrategyWithNoChecking>::proccess();
            return;
        case (NOTATION::PIECE_FEATURES::CAN_ATTACK_ON_LINES):
            GenerateRockMoves2<c, i, StrategyWithNoChecking>::proccess();
            return;
        case (NOTATION::PIECE_FEATURES::CAN_ATTACK_ON_DIAGONAL):
            GenerateBishopMoves2<c, i, StrategyWithNoChecking>::proccess();
            return;
        case (NOTATION::PIECES::QUEEN):
            GenerateQueenMoves2<c, i, StrategyWithNoChecking>::proccess();
            return;
        case (NOTATION::PIECES::KING):
            GenerateKingMoves2<c, i>::proccess();
    }
}

template <NOTATION::COLOR::color c>
void evaluateForCheckedPosition()
{
    dispatchToProperHandler<c, 0>();
    dispatchToProperHandler<c, 1>();
    dispatchToProperHandler<c, 2>();
    dispatchToProperHandler<c, 3>();
    dispatchToProperHandler<c, 4>();
    dispatchToProperHandler<c, 5>();
    dispatchToProperHandler<c, 6>();
    dispatchToProperHandler<c, 7>();
    dispatchToProperHandler<c, 8>();
    dispatchToProperHandler<c, 9>();
    dispatchToProperHandler<c, 10>();
    dispatchToProperHandler<c, 11>();
    dispatchToProperHandler<c, 12>();
    dispatchToProperHandler<c, 13>();
    dispatchToProperHandler<c, 14>();
    dispatchToProperHandler<c, 15>();
    dispatchToProperHandler<c, 16>();
    dispatchToProperHandler<c, 17>();
    dispatchToProperHandler<c, 18>();
    dispatchToProperHandler<c, 19>();
    dispatchToProperHandler<c, 20>();
    dispatchToProperHandler<c, 21>();
    dispatchToProperHandler<c, 22>();
    dispatchToProperHandler<c, 23>();
    dispatchToProperHandler<c, 24>();
    dispatchToProperHandler<c, 25>();
    dispatchToProperHandler<c, 26>();
    dispatchToProperHandler<c, 27>();
    dispatchToProperHandler<c, 28>();
    dispatchToProperHandler<c, 29>();
    dispatchToProperHandler<c, 30>();
    dispatchToProperHandler<c, 31>();
    dispatchToProperHandler<c, 32>();
    dispatchToProperHandler<c, 33>();
    dispatchToProperHandler<c, 34>();
    dispatchToProperHandler<c, 35>();
    dispatchToProperHandler<c, 36>();
    dispatchToProperHandler<c, 37>();
    dispatchToProperHandler<c, 38>();
    dispatchToProperHandler<c, 39>();
    dispatchToProperHandler<c, 40>();
    dispatchToProperHandler<c, 41>();
    dispatchToProperHandler<c, 42>();
    dispatchToProperHandler<c, 43>();
    dispatchToProperHandler<c, 44>();
    dispatchToProperHandler<c, 45>();
    dispatchToProperHandler<c, 46>();
    dispatchToProperHandler<c, 47>();
    dispatchToProperHandler<c, 48>();
    dispatchToProperHandler<c, 49>();
    dispatchToProperHandler<c, 50>();
    dispatchToProperHandler<c, 51>();
    dispatchToProperHandler<c, 52>();
    dispatchToProperHandler<c, 53>();
    dispatchToProperHandler<c, 54>();
    dispatchToProperHandler<c, 55>();
    dispatchToProperHandler<c, 56>();
    dispatchToProperHandler<c, 57>();
    dispatchToProperHandler<c, 58>();
    dispatchToProperHandler<c, 59>();
    dispatchToProperHandler<c, 60>();
    dispatchToProperHandler<c, 61>();
    dispatchToProperHandler<c, 62>();
    dispatchToProperHandler<c, 63>();
}

template <NOTATION::COLOR::color c>
void evaluateNotCheckedPostions(uint64_t pinnedMask)
{
    if (not ((1lu << 0) & pinnedMask)) generateWithAllMoveAllowance<c, 0>(); else dispatchToProperHandler<c, 0>();
    if (not ((1lu << 1) & pinnedMask)) generateWithAllMoveAllowance<c, 1>(); else dispatchToProperHandler<c, 1>();
    if (not ((1lu << 2) & pinnedMask)) generateWithAllMoveAllowance<c, 2>(); else dispatchToProperHandler<c, 2>();
    if (not ((1lu << 3) & pinnedMask)) generateWithAllMoveAllowance<c, 3>(); else dispatchToProperHandler<c, 3>();
    if (not ((1lu << 4) & pinnedMask)) generateWithAllMoveAllowance<c, 4>(); else dispatchToProperHandler<c, 4>();
    if (not ((1lu << 5) & pinnedMask)) generateWithAllMoveAllowance<c, 5>(); else dispatchToProperHandler<c, 5>();
    if (not ((1lu << 6) & pinnedMask)) generateWithAllMoveAllowance<c, 6>(); else dispatchToProperHandler<c, 6>();
    if (not ((1lu << 7) & pinnedMask)) generateWithAllMoveAllowance<c, 7>(); else dispatchToProperHandler<c, 7>();
    if (not ((1lu << 8) & pinnedMask)) generateWithAllMoveAllowance<c, 8>(); else dispatchToProperHandler<c, 8>();
    if (not ((1lu << 9) & pinnedMask)) generateWithAllMoveAllowance<c, 9>(); else dispatchToProperHandler<c, 9>();
    if (not ((1lu << 10) & pinnedMask)) generateWithAllMoveAllowance<c, 10>(); else dispatchToProperHandler<c, 10>();
    if (not ((1lu << 11) & pinnedMask)) generateWithAllMoveAllowance<c, 11>(); else dispatchToProperHandler<c, 11>();
    if (not ((1lu << 12) & pinnedMask)) generateWithAllMoveAllowance<c, 12>(); else dispatchToProperHandler<c, 12>();
    if (not ((1lu << 13) & pinnedMask)) generateWithAllMoveAllowance<c, 13>(); else dispatchToProperHandler<c, 13>();
    if (not ((1lu << 14) & pinnedMask)) generateWithAllMoveAllowance<c, 14>(); else dispatchToProperHandler<c, 14>();
    if (not ((1lu << 15) & pinnedMask)) generateWithAllMoveAllowance<c, 15>(); else dispatchToProperHandler<c, 15>();
    if (not ((1lu << 16) & pinnedMask)) generateWithAllMoveAllowance<c, 16>(); else dispatchToProperHandler<c, 16>();
    if (not ((1lu << 17) & pinnedMask)) generateWithAllMoveAllowance<c, 17>(); else dispatchToProperHandler<c, 17>();
    if (not ((1lu << 18) & pinnedMask)) generateWithAllMoveAllowance<c, 18>(); else dispatchToProperHandler<c, 18>();
    if (not ((1lu << 19) & pinnedMask)) generateWithAllMoveAllowance<c, 19>(); else dispatchToProperHandler<c, 19>();
    if (not ((1lu << 20) & pinnedMask)) generateWithAllMoveAllowance<c, 20>(); else dispatchToProperHandler<c, 20>();
    if (not ((1lu << 21) & pinnedMask)) generateWithAllMoveAllowance<c, 21>(); else dispatchToProperHandler<c, 21>();
    if (not ((1lu << 22) & pinnedMask)) generateWithAllMoveAllowance<c, 22>(); else dispatchToProperHandler<c, 22>();
    if (not ((1lu << 23) & pinnedMask)) generateWithAllMoveAllowance<c, 23>(); else dispatchToProperHandler<c, 23>();
    if (not ((1lu << 24) & pinnedMask)) generateWithAllMoveAllowance<c, 24>(); else dispatchToProperHandler<c, 24>();
    if (not ((1lu << 25) & pinnedMask)) generateWithAllMoveAllowance<c, 25>(); else dispatchToProperHandler<c, 25>();
    if (not ((1lu << 26) & pinnedMask)) generateWithAllMoveAllowance<c, 26>(); else dispatchToProperHandler<c, 26>();
    if (not ((1lu << 27) & pinnedMask)) generateWithAllMoveAllowance<c, 27>(); else dispatchToProperHandler<c, 27>();
    if (not ((1lu << 28) & pinnedMask)) generateWithAllMoveAllowance<c, 28>(); else dispatchToProperHandler<c, 28>();
    if (not ((1lu << 29) & pinnedMask)) generateWithAllMoveAllowance<c, 29>(); else dispatchToProperHandler<c, 29>();
    if (not ((1lu << 30) & pinnedMask)) generateWithAllMoveAllowance<c, 30>(); else dispatchToProperHandler<c, 30>();
    if (not ((1lu << 31) & pinnedMask)) generateWithAllMoveAllowance<c, 31>(); else dispatchToProperHandler<c, 31>();
    if (not ((1lu << 32) & pinnedMask)) generateWithAllMoveAllowance<c, 32>(); else dispatchToProperHandler<c, 32>();
    if (not ((1lu << 33) & pinnedMask)) generateWithAllMoveAllowance<c, 33>(); else dispatchToProperHandler<c, 33>();
    if (not ((1lu << 34) & pinnedMask)) generateWithAllMoveAllowance<c, 34>(); else dispatchToProperHandler<c, 34>();
    if (not ((1lu << 35) & pinnedMask)) generateWithAllMoveAllowance<c, 35>(); else dispatchToProperHandler<c, 35>();
    if (not ((1lu << 36) & pinnedMask)) generateWithAllMoveAllowance<c, 36>(); else dispatchToProperHandler<c, 36>();
    if (not ((1lu << 37) & pinnedMask)) generateWithAllMoveAllowance<c, 37>(); else dispatchToProperHandler<c, 37>();
    if (not ((1lu << 38) & pinnedMask)) generateWithAllMoveAllowance<c, 38>(); else dispatchToProperHandler<c, 38>();
    if (not ((1lu << 39) & pinnedMask)) generateWithAllMoveAllowance<c, 39>(); else dispatchToProperHandler<c, 39>();
    if (not ((1lu << 40) & pinnedMask)) generateWithAllMoveAllowance<c, 40>(); else dispatchToProperHandler<c, 40>();
    if (not ((1lu << 41) & pinnedMask)) generateWithAllMoveAllowance<c, 41>(); else dispatchToProperHandler<c, 41>();
    if (not ((1lu << 42) & pinnedMask)) generateWithAllMoveAllowance<c, 42>(); else dispatchToProperHandler<c, 42>();
    if (not ((1lu << 43) & pinnedMask)) generateWithAllMoveAllowance<c, 43>(); else dispatchToProperHandler<c, 43>();
    if (not ((1lu << 44) & pinnedMask)) generateWithAllMoveAllowance<c, 44>(); else dispatchToProperHandler<c, 44>();
    if (not ((1lu << 45) & pinnedMask)) generateWithAllMoveAllowance<c, 45>(); else dispatchToProperHandler<c, 45>();
    if (not ((1lu << 46) & pinnedMask)) generateWithAllMoveAllowance<c, 46>(); else dispatchToProperHandler<c, 46>();
    if (not ((1lu << 47) & pinnedMask)) generateWithAllMoveAllowance<c, 47>(); else dispatchToProperHandler<c, 47>();
    if (not ((1lu << 48) & pinnedMask)) generateWithAllMoveAllowance<c, 48>(); else dispatchToProperHandler<c, 48>();
    if (not ((1lu << 49) & pinnedMask)) generateWithAllMoveAllowance<c, 49>(); else dispatchToProperHandler<c, 49>();
    if (not ((1lu << 50) & pinnedMask)) generateWithAllMoveAllowance<c, 50>(); else dispatchToProperHandler<c, 50>();
    if (not ((1lu << 51) & pinnedMask)) generateWithAllMoveAllowance<c, 51>(); else dispatchToProperHandler<c, 51>();
    if (not ((1lu << 52) & pinnedMask)) generateWithAllMoveAllowance<c, 52>(); else dispatchToProperHandler<c, 52>();
    if (not ((1lu << 53) & pinnedMask)) generateWithAllMoveAllowance<c, 53>(); else dispatchToProperHandler<c, 53>();
    if (not ((1lu << 54) & pinnedMask)) generateWithAllMoveAllowance<c, 54>(); else dispatchToProperHandler<c, 54>();
    if (not ((1lu << 55) & pinnedMask)) generateWithAllMoveAllowance<c, 55>(); else dispatchToProperHandler<c, 55>();
    if (not ((1lu << 56) & pinnedMask)) generateWithAllMoveAllowance<c, 56>(); else dispatchToProperHandler<c, 56>();
    if (not ((1lu << 57) & pinnedMask)) generateWithAllMoveAllowance<c, 57>(); else dispatchToProperHandler<c, 57>();
    if (not ((1lu << 58) & pinnedMask)) generateWithAllMoveAllowance<c, 58>(); else dispatchToProperHandler<c, 58>();
    if (not ((1lu << 59) & pinnedMask)) generateWithAllMoveAllowance<c, 59>(); else dispatchToProperHandler<c, 59>();
    if (not ((1lu << 60) & pinnedMask)) generateWithAllMoveAllowance<c, 60>(); else dispatchToProperHandler<c, 60>();
    if (not ((1lu << 61) & pinnedMask)) generateWithAllMoveAllowance<c, 61>(); else dispatchToProperHandler<c, 61>();
    if (not ((1lu << 62) & pinnedMask)) generateWithAllMoveAllowance<c, 62>(); else dispatchToProperHandler<c, 62>();
    if (not ((1lu << 63) & pinnedMask)) generateWithAllMoveAllowance<c, 63>(); else dispatchToProperHandler<c, 63>();
}

} // namespace MoveGenerator
