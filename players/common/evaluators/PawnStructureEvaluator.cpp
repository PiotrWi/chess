#include "PawnStructureEvaluator.hpp"

#include <publicIf/Board.hpp>
#include <publicIf/Notation.hpp>
#include <publicIf/NotationConversions.hpp>

namespace pawnStructureEvaluator
{

namespace
{

int evaluateForWhite(const Board & board)
{
    auto eval = 0;
    unsigned char pawnsInColumn[8] = {};

    for (unsigned char pos = 8u; pos < 48; ++pos)
    {
        if (board.getField(pos) == (NOTATION::PIECES::PAWN | NOTATION::COLOR::WHITE))
        {
            pawnsInColumn[NotationConversions::getColumnNum(pos)] += 1;
        }
    }

    // doubled Pawn
    for (auto col = 0u; col < 8; ++col)
    {
        if (pawnsInColumn[col] > 1)
        {
            eval += (pawnsInColumn[col] - 1) * (-50);
        }
    }
    return eval;
}

int evaluateForBlack(const Board& board)
{
    auto eval = 0;
    unsigned char pawnsInColumn[8] = {};

    for (unsigned char pos = 8u; pos < 48; ++pos)
    {
        if (board.getField(pos) == (NOTATION::PIECES::PAWN | NOTATION::COLOR::BLACK))
        {
            pawnsInColumn[NotationConversions::getColumnNum(pos)] += 1;
        }
    }

    // doubled Pawn
    for (auto col = 0u; col < 8; ++col)
    {
        if (pawnsInColumn[col] > 1)
        {
            eval += (pawnsInColumn[col] - 1) * (-50);
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