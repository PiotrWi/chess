#pragma once

#include <BoardEngine.hpp>

struct EvaluationSettings
{
    bool materialEvaluation = true;
    bool pawnStructureEvaluation = true;
    bool moveCountEvaluation = true;
public:
    void switchOfAll();
};

extern EvaluationSettings evaluationSettings;

int evaluateFunction(BoardEngine& be, unsigned int validMovesCount);
