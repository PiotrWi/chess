#include "BoardEngine.hpp"
#include <detail/MoveValidator.hpp>
#include <detail/MoveGenerator.hpp>
#include <hashing/zobrist.hpp>

BoardEngine::BoardEngine()
{
    initDefault(board);
    hash_ = hash::hash(board);
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
    MoveApplier::applyMove(board, hash_, move, resultEvaluator);
}

Result BoardEngine::getResult()
{
    return resultEvaluator.evaluate();
}

Result BoardEngine::getResult(bool availableMoves)
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
    return MoveApplier::applyTmpMove(board, hash_, move, resultEvaluator);
}

void BoardEngine::undoMove(const MoveApplier::MoveMemorial moveMemorial)
{
    MoveApplier::undoMove(board, hash_, moveMemorial, resultEvaluator);
}

MoveApplier::SimpleMoveMemorial BoardEngine::applyUndoableSimpleMove(const Move& move)
{
    return MoveApplier::applyTmpMoveSimple(board, hash_, move, resultEvaluator);
}

void BoardEngine::undoMove(MoveApplier::SimpleMoveMemorial sm)
{
    return MoveApplier::undoMove(board, hash_, sm, resultEvaluator);
}

uint64_t BoardEngine::getHash() const
{
    return hash_;
}