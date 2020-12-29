#include "MoveCountEvaluator.hpp"
#include <publicIf/NotationConversions.hpp>

namespace moveCountEvaluator
{

int evaluate(int playerOnMoveMovesCount, BoardEngine be)
{
    auto oponentMovesCount = be.generateMovesFor(be.board.playerOnMove + 1).size();
    return (playerOnMoveMovesCount - oponentMovesCount) * 10;
}

}