#pragma once

#include <detail/MoveApplier.hpp>
#include <detail/CheckChecker.hpp>

namespace MoveGenerator
{

bool isChecked(const ExtendedMove& move);
bool isKingCheckedAfterMove(const ExtendedMove& move);
bool allowAll(const ExtendedMove&);

bool isChecked(const ExtendedMove& move)
{
    Board boardCopy = *ctx.board;
    MoveApplier::applyMove(boardCopy, move);
    return not CheckChecker::isAttackedOn(boardCopy, ctx.board->playerOnMove, ctx.kingPosition);
}

bool isKingCheckedAfterMove(const ExtendedMove& move)
{
    Board boardCopy = *ctx.board;
    MoveApplier::applyMove(boardCopy, move);
    return not CheckChecker::isAttackedOn(boardCopy, ctx.board->playerOnMove, move.destination);
}

bool allowAll(const ExtendedMove&)
{
    return true;
}

template <bool (*isValid)(const ExtendedMove&), bool (*isValidForKing)(const ExtendedMove&), NOTATION::COLOR::color c>
class AddingStrategy
{
public:
    static void addAndPromote(unsigned char source, unsigned char destination)
    {
        constexpr unsigned char MASK = ExtendedMove::promotionMask | ExtendedMove::pawnMoveMask;
        constexpr unsigned char c_bin = static_cast<unsigned char>(c);
        constexpr unsigned char pawn = NOTATION::PIECES::PAWN | c_bin;

        ExtendedMove m = ExtendedMove(source, destination,
                                      MASK,
                                      NOTATION::PIECES::QUEEN | c_bin,
                                      pawn, 0);
        if (not isValid(m))
        {
            return;
        }
        ctx.promotions[ctx.Npromotions++] = m;
        ctx.pawnsMoves[ctx.NPawnsMoves++] =  ExtendedMove(source, destination, MASK, NOTATION::PIECES::BISHOP | c_bin, pawn, 0);
        ctx.pawnsMoves[ctx.NPawnsMoves++] =  ExtendedMove(source, destination, MASK, NOTATION::PIECES::ROCK | c_bin, pawn, 0);
        ctx.pawnsMoves[ctx.NPawnsMoves++] =  ExtendedMove(source, destination, MASK, NOTATION::PIECES::KNIGHT | c_bin, pawn, 0);
    }

    static void addAndPromoteWithBeating(unsigned char source, unsigned char destination, unsigned char targetField)
    {
        constexpr unsigned char MASK = ExtendedMove::promotionMask
                                       | ExtendedMove::pawnMoveMask
                                       | ExtendedMove::beatingMask;
        constexpr unsigned char c_bin = static_cast<unsigned char>(c);
        unsigned char pawn = NOTATION::PIECES::PAWN | c_bin;

        ExtendedMove m = ExtendedMove(source, destination,
                                      MASK,
                                      NOTATION::PIECES::QUEEN | c_bin,
                                      pawn, targetField);
        if (not isValid(m))
        {
            return;
        }

        ctx.beatings[ctx.Nbeatings++] = m;
        ctx.pawnsMoves[ctx.NPawnsMoves++] =  ExtendedMove(source, destination, MASK, NOTATION::PIECES::BISHOP | c_bin, pawn, targetField);
        ctx.pawnsMoves[ctx.NPawnsMoves++] =  ExtendedMove(source, destination, MASK, NOTATION::PIECES::ROCK | c_bin, pawn, targetField);
        ctx.pawnsMoves[ctx.NPawnsMoves++] =  ExtendedMove(source, destination, MASK, NOTATION::PIECES::KNIGHT | c_bin, pawn, targetField);
    }

    static void addPawn(unsigned char source, unsigned char destination)
    {
        constexpr unsigned char MASK = ExtendedMove::pawnMoveMask;
        constexpr unsigned char pawn = NOTATION::PIECES::PAWN | static_cast<unsigned char>(c);

        ExtendedMove m = ExtendedMove(source, destination, MASK, 0, pawn, 0);
        if (not isValid(m))
        {
            return;
        }
        ctx.pawnsMoves[ctx.NPawnsMoves++] = m;
    }

    static void addPawnWithBeating(unsigned char source, unsigned char destination, unsigned char targetField)
    {
        constexpr unsigned char MASK = ExtendedMove::pawnMoveMask | ExtendedMove::beatingMask;

        constexpr unsigned char pawn = NOTATION::PIECES::PAWN | static_cast<unsigned char>(c);

        ExtendedMove m = ExtendedMove(source, destination, MASK, 0, pawn, targetField);
        if (not isValid(m))
        {
            return;
        }
        ctx.beatings[ctx.Nbeatings++] = m;
    }

    static void addKnight(unsigned char source, unsigned char destination)
    {
        constexpr unsigned char MASK = 0;
        constexpr unsigned char knight = NOTATION::PIECES::KNIGHT | static_cast<unsigned char>(c);

        ExtendedMove m = ExtendedMove(source, destination, MASK, 0, knight, 0);
        if (not isValid(m))
        {
            return;
        }
        ctx.figuresMoves[ctx.NFiguresMoves++] = m;
    }

    static void addKnightWithBeating(unsigned char source, unsigned char destination, unsigned char targetField)
    {
        constexpr unsigned char MASK = ExtendedMove::beatingMask;
        constexpr unsigned char knight = NOTATION::PIECES::KNIGHT | static_cast<unsigned char>(c);

        ExtendedMove m = ExtendedMove(source, destination, MASK, 0, knight, targetField);
        if (not isValid(m))
        {
            return;
        }
        ctx.beatings[ctx.Nbeatings++] = m;
    }

    static void addKing(unsigned char source, unsigned char destination)
    {
        constexpr unsigned char MASK = ExtendedMove::kingMoveMask;
        constexpr unsigned char king = NOTATION::PIECES::KING | static_cast<unsigned char>(c);

        ExtendedMove m = ExtendedMove(source, destination, MASK, 0, king, 0);
        if (not isValidForKing(m))
        {
            return;
        }
        ctx.kingMoves[ctx.NKingMoves++] = m;
    }

    static void addKingWithBeating(unsigned char source, unsigned char destination, unsigned char targetField)
    {
        constexpr unsigned char MASK = ExtendedMove::kingMoveMask | ExtendedMove::beatingMask;
        constexpr unsigned char knight = NOTATION::PIECES::KING | static_cast<unsigned char>(c);

        ExtendedMove m = ExtendedMove(source, destination, MASK, 0, knight, targetField);
        if (not isValidForKing(m))
        {
            return;
        }
        ctx.beatings[ctx.Nbeatings++] = m;
    }

    static void addRock(unsigned char source, unsigned char destination)
    {
        constexpr unsigned char MASK = 0;
        constexpr unsigned char rock = NOTATION::PIECES::ROCK | static_cast<unsigned char>(c);

        ExtendedMove m = ExtendedMove(source, destination, MASK, 0, rock, 0);
        if (not isValid(m))
        {
            return;
        }
        ctx.figuresMoves[ctx.NFiguresMoves++] = m;
    }

    static void addRockWithBeating(unsigned char source, unsigned char destination, unsigned char targetField)
    {
        constexpr unsigned char MASK = ExtendedMove::beatingMask;
        constexpr unsigned char rock = NOTATION::PIECES::ROCK | static_cast<unsigned char>(c);

        ExtendedMove m = ExtendedMove(source, destination, MASK, 0, rock, targetField);
        if (not isValid(m))
        {
            return;
        }
        ctx.beatings[ctx.Nbeatings++] = m;
    }

    static void addBishop(unsigned char source, unsigned char destination)
    {
        constexpr unsigned char MASK = 0;
        constexpr unsigned char bishop = NOTATION::PIECES::BISHOP | static_cast<unsigned char>(c);

        ExtendedMove m = ExtendedMove(source, destination, MASK, 0, bishop, 0);
        if (not isValid(m))
        {
            return;
        }
        ctx.figuresMoves[ctx.NFiguresMoves++] = m;
    }

    static void addBishopWithBeating(unsigned char source, unsigned char destination, unsigned char targetField)
    {
        constexpr unsigned char MASK = ExtendedMove::beatingMask;
        constexpr unsigned char bishop = NOTATION::PIECES::BISHOP | static_cast<unsigned char>(c);

        ExtendedMove m = ExtendedMove(source, destination, MASK, 0, bishop, targetField);
        if (not isValid(m))
        {
            return;
        }
        ctx.beatings[ctx.Nbeatings++] = m;
    }

    static void addQueen(unsigned char source, unsigned char destination)
    {
        constexpr unsigned char MASK = 0;
        constexpr unsigned char queen = NOTATION::PIECES::QUEEN | static_cast<unsigned char>(c);

        ExtendedMove m = ExtendedMove(source, destination, MASK, 0, queen, 0);
        if (not isValid(m))
        {
            return;
        }
        ctx.figuresMoves[ctx.NFiguresMoves++] = m;
    }

    static void addQueenWithBeating(unsigned char source, unsigned char destination, unsigned char targetField)
    {
        constexpr unsigned char MASK = ExtendedMove::beatingMask;
        constexpr unsigned char queen = NOTATION::PIECES::QUEEN | static_cast<unsigned char>(c);

        ExtendedMove m = ExtendedMove(source, destination, MASK, 0, queen, targetField);
        if (not isValid(m))
        {
            return;
        }
        ctx.beatings[ctx.Nbeatings++] = m;
    }
};

template<NOTATION::COLOR::color c>
using StrategyWithAlwaysCheckChecking = AddingStrategy<isChecked, isKingCheckedAfterMove, c>;

template<NOTATION::COLOR::color c>
using StrategyWithNoChecking = AddingStrategy<allowAll, allowAll, c>;

}
