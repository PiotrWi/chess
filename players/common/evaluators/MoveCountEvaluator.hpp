#pragma once

namespace moveCountEvaluator
{
template <typename COEF>
int evaluate(int playerOnMoveMovesCount, int oponentMovesCount)
{
    return (playerOnMoveMovesCount - oponentMovesCount) * COEF::moveValue;
}
}