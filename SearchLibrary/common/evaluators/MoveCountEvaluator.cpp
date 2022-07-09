#include "MoveCountEvaluator.hpp"

namespace moveCountEvaluator
{

int evaluate(int playerOnMoveMovesCount, int oponentMovesCount)
{
    return (playerOnMoveMovesCount - oponentMovesCount) * 10;
}

}