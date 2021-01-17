#pragma once

#include <BoardEngine.hpp>
#include <publicIf/NotationConversions.hpp>
#include <common/evaluators/MoveCountEvaluator.hpp>
#include <common/evaluators/MaterialEvaluator.hpp>
#include <common/evaluators/PawnStructureEvaluator.hpp>

static int evaluateFunction(const BoardEngine& be)
{
    auto validMoves = be.generateMoves();
    auto gameResult = be.getREsultWithoutRepeatitionCheck(not validMoves.empty());
    if ((gameResult == Result::whiteWon) | (gameResult == Result::blackWon))
    {
        return -10000000;
    }

    auto oponentValidMoves = be.generateMovesFor(be.board.playerOnMove + 1);
    return materialEvaluator::evaluate(be.board, be.board.playerOnMove)
        + moveCountEvaluator::evaluate(validMoves.size(), oponentValidMoves.size())
        + pawnStructureEvaluator::evaluate(be.board, be.board.playerOnMove);
}