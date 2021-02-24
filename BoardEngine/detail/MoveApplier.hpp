#pragma once

#include "publicIf/Board.hpp"
#include <cstdint>

class ResultEvaluator;

namespace MoveApplier
{

struct SimpleMoveMemorial
{
    Board previousBoard;
    uint64_t positionHash;
};

void applyMove(Board& board, const ExtendedMove& move);
void applyMove(Board&, uint64_t& positionHash, const ExtendedMove&);

SimpleMoveMemorial applyTmpMoveSimple(Board&, uint64_t& positionHash, const ExtendedMove&);

template <class TResultEvaluator>
void applyMove(Board& board,
        uint64_t& positionHash,
        const ExtendedMove& move,
        TResultEvaluator& resultEvaluator)
{
    applyMove(board, positionHash, move);
    resultEvaluator.storeBoard(board, move);
}

template <class TResultEvaluator>
SimpleMoveMemorial applyTmpMoveSimple(Board& board,
        uint64_t& positionHash,
        const ExtendedMove& move,
        TResultEvaluator& resultEvaluator)
{
    auto memorial = applyTmpMoveSimple(board, positionHash, move);
    resultEvaluator.storeBoard(board, move);
    return memorial;
}

void undoMove(Board& board, uint64_t& positionHash, const SimpleMoveMemorial& memorial);
template <class TResultEvaluator> void undoMove(Board& board, uint64_t& positionHash, const SimpleMoveMemorial& memorial, TResultEvaluator& resultEvaluator)
{
    resultEvaluator.removeSingle();
    undoMove(board, positionHash, memorial);
}

}  // namespace MoveApplier
