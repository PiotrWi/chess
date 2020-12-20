#include "BoardEngine.hpp"
#include <detail/MoveValidator.hpp>
#include <detail/MoveApplier.hpp>

BoardEngine::BoardEngine()
{
    initDefault(board);
}

bool BoardEngine::validateMove(const Move& move)
{
    return MoveValidator::validateMove(board, move);
}

void BoardEngine::applyMove(const Move& move)
{
    MoveApplier::applyMove(board, move, resultEvaluator);
}

Result BoardEngine::getResult()
{
    return resultEvaluator.evaluate();
}