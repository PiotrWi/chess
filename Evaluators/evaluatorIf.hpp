#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP

#include <BoardEngine.hpp>

extern "C"
{

void init(const char* configurationFile);
int evaluatePosition(BoardEngine& be, unsigned int validMovesCount);
int evaluatePositionAvx(BoardEngine& be, unsigned int validMovesCount);

}

#endif // EVALUATOR_HPP
