#pragma once

#include <vector>

#include "core/Board.hpp"
#include "ResultEvaluator/Result.hpp"

#include "MoveApplier/MoveApplier.hpp"
#include "ResultEvaluator/ResultEvaluator.hpp"
#include <MoveGeneratorV2/MoveGeneratorV2.hpp>

class BoardEngine
{
public:
    BoardEngine();
    BoardEngine(const BoardEngine&);
    BoardEngine& operator=(const BoardEngine&);
    explicit BoardEngine(const Board& board);

    bool validateMove(const Move&) const;
    bool isChecked() const;

    MoveApplier::SimpleMoveMemorial getMemorial() const;
    void applyMove(const ExtendedMove&);
    MoveApplier::SimpleMoveMemorial applyUndoableSimpleMove(const ExtendedMove&);
    void undoMove(MoveApplier::SimpleMoveMemorial);
    uint64_t getHash() const;
    uint64_t getHash(NOTATION::COLOR::color) const;

// Result evaluators
    Result getResult();
    Result getResult(bool availableMoves);
    bool are3Repeatitions();
    Result getREsultWithoutRepeatitionCheck(bool availableMoves) const;

// MovesGenerationV2
    MoveGenerator::MoveGeneratorV2 getMoveGeneratorV2(const NOTATION::COLOR::color c) const;

    Board board;
    ExtendedMove lastMove = {};
    ResultEvaluator resultEvaluator;
    uint64_t hash_ = 0lu;
};