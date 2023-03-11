#include "BoardEngine.hpp"
#include <cassert>
#include <map>
#include <set>
#include <iostream>
#include "MoveValidator/MoveValidator.hpp"
#include "core/BoardIO.hpp"
#include "PositionAnalyzers/CheckChecker.hpp"
#include <hashing/zobrist.hpp>
#include <notations/coordinationnotation.hpp>

BoardEngine::BoardEngine()
{
    initDefault(board);
    hash_ = hash::hash(board);
}

BoardEngine::BoardEngine(const BoardEngine& other)
    : board(other.board)
    , resultEvaluator(other.resultEvaluator)
    , hash_(other.hash_)
{
#ifdef ASSERTSON
    if (hash::hash(board) != hash_)
    {
        std::cout << hash::hash(board) << " " << hash_ << std::endl;
    }
    assert(hash::hash(board) == hash_);
#endif
}

BoardEngine::BoardEngine(const Board& b)
        : board(b)
        , hash_(hash::hash(b))
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

bool BoardEngine::isChecked() const
{
    return CheckChecker::isCheckOn(board, board.playerOnMove);
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

void BoardEngine::applyMove(const ExtendedMove& move)
{
#ifdef ASSERTSON
    assert(board.isCorrect());
auto oldBoard = board;
#endif

    MoveApplier::applyMove(board, hash_, move, resultEvaluator);

#ifdef ASSERTSON
    if (hash::hash(board) != hash_)
    {
        std::cout << "Internal consistency error!";
        std::cout << hash::hash(board) << " " << hash_ << std::endl;

        std::cout << "Old board was:" << std::endl;
        std::cout << oldBoard << std::endl;

        std::cout << "Move is: " << move.operator Move();

        std::cout << "After move board is:" << std::endl;
        std::cout << board << std::endl;
    }
    assert(board.isCorrect());
    assert(hash::hash(board) == hash_);
#endif
#ifdef ASSERTSON
    static unsigned counter = 0;
    static std::map<decltype(hash_), std::vector<Board>> hashConflictsMap;

    if (hashConflictsMap[hash_].end() == std::find(
            hashConflictsMap[hash_].begin(),
            hashConflictsMap[hash_].end(),
            board))
    {
        hashConflictsMap[hash_].push_back(board);
        if (hashConflictsMap[hash_].size() > 1)
        {
            ++counter;
            std::cout << "Num of hash conflicts: " << counter << "/" << hashConflictsMap.size() << std::endl;
        }
    }
#endif
    lastMove = move;
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

MoveGenerator::MoveGeneratorV2 BoardEngine::getMoveGeneratorV2(const NOTATION::COLOR::color c) const
{
    return MoveGenerator::MoveGeneratorV2(board, c);
}

