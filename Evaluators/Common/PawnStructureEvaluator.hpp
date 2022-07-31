#pragma once

#include <publicIf/Notation.hpp>
#include <publicIf/NotationConversions.hpp>
#include <detail/bitboardslookups.hpp>

struct PawnStructureCoefficients
{
    int multiplePawnInRank = -50;
    int isolatedPawn = -50;
};

namespace pawn_structure_evaluator
{

template <unsigned char TColor>
int evaluatePawnStructure(const Board & board, const PawnStructureCoefficients& coeffincients)
{
    auto eval = 0;

    unsigned char pawnsPerRank[8];

    for (auto i = 0u; i < 8; ++i)
    {
    	pawnsPerRank[i] = __builtin_popcountll(ranks[i] & board.piecesBitSets[TColor].pawnsMask);
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
        eval += (-50);
    }
    if (pawnsPerRank[7] > 0 && pawnsPerRank[6] == 0)
    {
        eval += (-50);
    }
    for (auto i = 1u; i < 7; ++i)
    {
        if (pawnsPerRank[i] > 1 && pawnsPerRank[i - 1] == 0 && pawnsPerRank[i + 1] == 0)
        {
            eval += (pawnsPerRank[i] - 1) * (-50);
        }
    }

    return eval;
}

static int (*evaluatePawnStructureForWhite)(const Board&, const PawnStructureCoefficients& coeffincients) =  evaluatePawnStructure<NOTATION::COLOR::WHITE>;
static int (*evaluatePawnStructureForBlack)(const Board&, const PawnStructureCoefficients& coeffincients) =  evaluatePawnStructure<NOTATION::COLOR::BLACK>;

int evaluatePawnStructure(const Board& board, NOTATION::COLOR::color playerOnMove, const PawnStructureCoefficients& coeffincients)
{
    auto val = evaluatePawnStructureForWhite(board, coeffincients) - evaluatePawnStructureForBlack(board, coeffincients);
    return (playerOnMove == NOTATION::COLOR::color::white)
        ? val
        : -1 * val;
}

}  // namespace pawn_structure_evaluator
