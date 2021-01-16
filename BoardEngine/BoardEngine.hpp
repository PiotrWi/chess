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

    MoveApplier::SimpleMoveMemorial getMemorial() const;
    void applyMove(const Move&);
    MoveApplier::SimpleMoveMemorial applyUndoableSimpleMove(const Move&);
    void undoMove(MoveApplier::SimpleMoveMemorial);
    uint64_t getHash() const;
    uint64_t getHash(NOTATION::COLOR::color) const;

// Result evaluators
    Result getResult();
    Result getResult(bool availableMoves);
    bool are3Repeatitions();
    Result getREsultWithoutRepeatitionCheck(bool availableMoves) const;

// MovesGeneration
    unsigned generateValidMoveCount() const;
    unsigned generateValidMoveCount(NOTATION::COLOR::color) const;
    std::vector<ExtendedMove> generateMoves() const;
    std::vector<ExtendedMove> generateMovesFor(NOTATION::COLOR::color) const;

    Board board;
    ResultEvaluator resultEvaluator;
    uint64_t hash_ = 0lu;
};