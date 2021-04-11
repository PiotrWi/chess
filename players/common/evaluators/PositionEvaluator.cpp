#include "PositionEvaluator.hpp"

#include <publicIf/NotationConversions.hpp>
#include <common/evaluators/MoveCountEvaluator.hpp>
#include <common/evaluators/MaterialEvaluator.hpp>
#include <common/evaluators/PawnStructureEvaluator.hpp>

EvaluationSettings evaluationSettings;

void EvaluationSettings::switchOfAll()
{
    materialEvaluation = false;
    pawnStructureEvaluation = false;
    moveCountEvaluation = false;
}

int evaluateFunction(BoardEngine& be, unsigned int validMovesCount)
{
    auto gameResult = be.getREsultWithoutRepeatitionCheck(validMovesCount);
    if ((gameResult == Result::whiteWon) | (gameResult == Result::blackWon))
    {
        return -10000000;
    }

    int evaluationValue = 0;
    if (evaluationSettings.materialEvaluation)
    {
        evaluationValue += materialEvaluator::evaluate(be.board, be.board.playerOnMove);
    }
    if (evaluationSettings.moveCountEvaluation)
    {
        auto oponentValidMoves = be.generateValidMoveCount(be.board.playerOnMove + 1);
        evaluationValue += moveCountEvaluator::evaluate(validMovesCount, oponentValidMoves);
    }
    if (evaluationSettings.pawnStructureEvaluation)
    {
        evaluationValue += pawnStructureEvaluator::evaluate(be.board, be.board.playerOnMove);
    }
    return evaluationValue;
}
