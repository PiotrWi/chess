#include "PawnStructureEvaluator.hpp"

#include <publicIf/Board.hpp>
#include <detail/bitboardslookups.hpp>

#include <publicIf/Notation.hpp>
#include <publicIf/NotationConversions.hpp>

namespace pawnStructureEvaluator
{

namespace
{

int evaluateForWhite(const Board & board)
{
    auto eval = 0;

    for (auto && rank : ranks)
    {
        auto pawnsInRank = __builtin_popcountll(
            rank & board.piecesBitSets[NOTATION::COLOR::WHITE].pawnsMask);
        if (pawnsInRank > 1)
        {
            eval += (pawnsInRank - 1) * (-50);
        }
    }

    return eval;
}

int evaluateForBlack(const Board& board)
{
    auto eval = 0;

    for (auto && rank : ranks)
    {
        auto pawnsInRank = __builtin_popcountll(
                rank & board.piecesBitSets[NOTATION::COLOR::BLACK].pawnsMask);
        if (pawnsInRank > 1)
        {
            eval += (pawnsInRank - 1) * (-50);
        }
    }

    return eval;
}

}  // namespace

int evaluate(const Board& board, NOTATION::COLOR::color playerOnMove)
{
    auto val = evaluateForWhite(board) - evaluateForBlack(board);
    return (playerOnMove == NOTATION::COLOR::color::white)
        ? val
        : -1 * val;
}

}  // namespace pawnStructureEvaluator