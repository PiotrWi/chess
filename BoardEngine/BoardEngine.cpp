#include "BoardEngine.hpp"
#include <detail/MoveValidator.hpp>
#include <detail/MoveGenerator.hpp>

BoardEngine::BoardEngine()
{
    initDefault(board);
}

BoardEngine::BoardEngine(const BoardEngine& other)
    : board(other.board)
    , resultEvaluator(other.resultEvaluator)
{
}

bool BoardEngine::validateMove(const Move& move) const
{
    return MoveValidator::validateMove(board, move);
}

void BoardEngine::applyMove(const Move& move)
{
    MoveApplier::applyMove(board, move, resultEvaluator);
}

Result BoardEngine::getResult() const
{
    return resultEvaluator.evaluate();
}

Result BoardEngine::getResult(bool availableMoves) const
{
    return resultEvaluator.evaluate(availableMoves);
}

std::vector<Move> BoardEngine::generateMoves() const
{
    return MoveGenerator::MoveGenerator::generate(board);
}

std::vector<Move> BoardEngine::generateMovesFor(NOTATION::COLOR::color color) const
{
    return MoveGenerator::MoveGenerator::generate(board, color);
}

MoveApplier::MoveMemorial BoardEngine::applyUndoableMove(const Move & move)
{
    return MoveApplier::applyTmpMove(board, move, resultEvaluator);
}

void BoardEngine::undoMove(const MoveApplier::MoveMemorial moveMemorial)
{
    MoveApplier::undoMove(board, moveMemorial, resultEvaluator);
}

MoveApplier::SimpleMoveMemorial BoardEngine::applyUndoableSimpleMove(const Move& move)
{
    return MoveApplier::applyTmpMoveSimple(board, move, resultEvaluator);
}

void BoardEngine::undoMove(MoveApplier::SimpleMoveMemorial sm)
{
    return MoveApplier::undoMove(board, sm, resultEvaluator);
}

