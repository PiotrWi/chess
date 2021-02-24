#include "BoardEngine.hpp"
#include <detail/MoveValidator.hpp>
#include <MoveGenerator/MoveGenerator.hpp>
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

BoardEngine& BoardEngine::operator=(const BoardEngine& be)
{
    if (&be == this)
    {
        return *this;
    }
    this->board = be.board;
    resultEvaluator = be.resultEvaluator;
    hash_ = be.hash_;
    return *this;
}


bool BoardEngine::validateMove(const Move& move) const
{
    return MoveValidator::validateMove(board, move);
}


Result BoardEngine::getResult()
{
    return resultEvaluator.evaluate();
}

Result BoardEngine::getResult(bool availableMoves)
{
    return resultEvaluator.evaluate(availableMoves);
}

bool BoardEngine::are3Repeatitions()
{
    return resultEvaluator.isDrawByRepeatitions();
}

Result BoardEngine::getREsultWithoutRepeatitionCheck(bool availableMoves) const
{
    return resultEvaluator.getResultNoRepeatitions(availableMoves);
}

unsigned BoardEngine::generateValidMoveCount() const
{
    return MoveGenerator::MoveGenerator::getMoveCount(board);
}

unsigned BoardEngine::generateValidMoveCount(NOTATION::COLOR::color c) const
{
    return MoveGenerator::MoveGenerator::getMoveCount(board, c);
}

std::vector<ExtendedMove> BoardEngine::generateMoves() const
{
    return MoveGenerator::MoveGenerator::generate(board);
}

std::vector<ExtendedMove> BoardEngine::generateMovesFor(NOTATION::COLOR::color color) const
{
    return MoveGenerator::MoveGenerator::generate(board, color);
}

void BoardEngine::applyMove(const ExtendedMove& move)
{
    MoveApplier::applyMove(board, hash_, move, resultEvaluator);
}

MoveApplier::SimpleMoveMemorial BoardEngine::applyUndoableSimpleMove(const ExtendedMove& move)
{
    return MoveApplier::applyTmpMoveSimple(board, hash_, move, resultEvaluator);
}

MoveApplier::SimpleMoveMemorial BoardEngine::getMemorial() const
{
    MoveApplier::SimpleMoveMemorial memorial {board, hash_};
    return memorial;
}

void BoardEngine::undoMove(MoveApplier::SimpleMoveMemorial sm)
{
    return MoveApplier::undoMove(board, hash_, sm, resultEvaluator);
}

uint64_t BoardEngine::getHash() const
{
    return hash_;
}

uint64_t BoardEngine::getHash(NOTATION::COLOR::color c) const
{
    if (c == board.playerOnMove)
    {
        return hash_;
    }
    return hash::switchColor(hash_);
}

