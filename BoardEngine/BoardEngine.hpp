#pragma once

#include <vector>

#include <publicIf/Board.hpp>
#include <publicIf/Result.hpp>

#include <detail/MoveApplier.hpp>
#include <detail/ResultEvaluator.hpp>

class BoardEngine
{
public:
    BoardEngine();
    BoardEngine(const BoardEngine&);

    bool validateMove(const Move&) const;

    void applyMove(const Move&);
    MoveApplier::MoveMemorial applyUndoableMove(const Move&);
// TODO: There is something wrong with move unoing
    void undoMove(const MoveApplier::MoveMemorial);

    MoveApplier::SimpleMoveMemorial applyUndoableSimpleMove(const Move&);
    void undoMove(MoveApplier::SimpleMoveMemorial);

    uint64_t getHash() const;

    Result getResult() const;
    Result getResult(bool availableMoves) const;
    std::vector<Move> generateMoves() const;
    std::vector<Move> generateMovesFor(NOTATION::COLOR::color) const;

    Board board;
    ResultEvaluator resultEvaluator;
    uint64_t hash_ = 0lu;
};