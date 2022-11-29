#pragma once

#include <publicIf/Notation.hpp>
#include <publicIf/NotationConversions.hpp>
#include <detail/bitboardslookups.hpp>

struct PawnStructureCoefficients
{
    int multiplePawnInRank = -50;
    int isolatedPawn = -50;
    int passedPawn = 50;
};

namespace pawn_structure_evaluator
{

template <NOTATION::COLOR::color TColor>
int evaluatePawnStructure(const Board & board, const PawnStructureCoefficients& coeffincients)
{
    auto eval = 0;

    unsigned char pawnsPerRank[8];
    unsigned char opponentsPawnPerRank[8];

    for (auto i = 0u; i < 8; ++i)
    {
        pawnsPerRank[i] = __builtin_popcountll(ranks[i] & board.piecesBitSets[static_cast<unsigned char>(TColor)].pawnsMask);
    }

    for (auto i = 0u; i < 8; ++i)
    {
        opponentsPawnPerRank[i] = __builtin_popcountll(ranks[i] & board.piecesBitSets[static_cast<unsigned char>(TColor+1)].pawnsMask);
    }

// doubled Pawns
    for (auto i = 0u; i < 8; ++i)
    {
        if (pawnsPerRank[i] > 1)
        {
            eval += (pawnsPerRank[i] - 1) * coeffincients.multiplePawnInRank;
        }
    }

// isolated Pawns
    if (pawnsPerRank[0] > 0 && pawnsPerRank[0] == 0)
    {
        eval += coeffincients.isolatedPawn;
    }
    if (pawnsPerRank[7] > 0 && pawnsPerRank[6] == 0)
    {
        eval += coeffincients.isolatedPawn;
    }
    for (auto i = 1u; i < 7; ++i)
    {
        if (pawnsPerRank[i] > 1 && pawnsPerRank[i - 1] == 0 && pawnsPerRank[i + 1] == 0)
        {
            eval += coeffincients.isolatedPawn;
        }
    }

    if (pawnsPerRank[0] > 0 && opponentsPawnPerRank[0] == 0 && opponentsPawnPerRank[1] == 0)
    {
        eval += coeffincients.passedPawn;
    }
    if (pawnsPerRank[7] > 0 && opponentsPawnPerRank[7] == 0 && opponentsPawnPerRank[6] == 0)
    {
        eval += coeffincients.passedPawn;
    }
    for (auto i = 1u; i < 7; ++i)
    {
        if (pawnsPerRank[i] > 1&& opponentsPawnPerRank[i-1] == 0 && opponentsPawnPerRank[i] == 0 && opponentsPawnPerRank[i+1] == 0)
        {
            eval += coeffincients.passedPawn;
        }
    }

    return eval;
}

static int (*evaluatePawnStructureForWhite)(const Board&, const PawnStructureCoefficients& coeffincients) =  evaluatePawnStructure<NOTATION::COLOR::color::white>;
static int (*evaluatePawnStructureForBlack)(const Board&, const PawnStructureCoefficients& coeffincients) =  evaluatePawnStructure<NOTATION::COLOR::color::black>;

int evaluatePawnStructure(const Board& board, NOTATION::COLOR::color playerOnMove, const PawnStructureCoefficients& coeffincients)
{
    auto val = evaluatePawnStructureForWhite(board, coeffincients) - evaluatePawnStructureForBlack(board, coeffincients);
    return (playerOnMove == NOTATION::COLOR::color::white)
        ? val
        : -1 * val;
}

}  // namespace pawn_structure_evaluator
