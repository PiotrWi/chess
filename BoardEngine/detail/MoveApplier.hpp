#pragma once

#include "publicIf/Board.hpp"
#include <cstdint>

class ResultEvaluator;

namespace MoveApplier
{

/* TODO To check what is faster.
 * Full copy of board or restoring board by MoveMemorial. */

struct MoveMemorial
{
    bool wasCasling;
    bool enPasant;
    unsigned char sourceField;
    unsigned char sourceVal;
    unsigned char targetField;
    unsigned char targetVal;
    uint64_t positionHash;
};

struct SimpleMoveMemorial
{
    Board previousBoard;
    uint64_t positionHash;
};

void applyMove(Board& board, const Move& move);

void applyMove(Board&, uint64_t& positionHash, const Move&);
MoveMemorial applyTmpMove(Board&, uint64_t& positionHash, const Move&);
SimpleMoveMemorial applyTmpMoveSimple(Board&, uint64_t& positionHash, const Move&);

template <class TResultEvaluator> void applyMove(Board& board, uint64_t& positionHash, const Move& move, TResultEvaluator& resultEvaluator)
{
    applyMove(board, positionHash, move);
    resultEvaluator.storeBoard(board);
}
template <class TResultEvaluator> MoveMemorial applyTmpMove(Board& board, uint64_t& positionHash, const Move& move, TResultEvaluator& resultEvaluator)
{
    auto memorial = applyTmpMove(board, positionHash, move);
    resultEvaluator.storeBoard(board);
    return memorial;
}

template <class TResultEvaluator> SimpleMoveMemorial applyTmpMoveSimple(Board& board,
        uint64_t& positionHash,
        const Move& move,
        TResultEvaluator& resultEvaluator)
{
    auto memorial = applyTmpMoveSimple(board, positionHash, move);
    resultEvaluator.storeBoard(board);
    return memorial;
}

// TODO: There is something wrong with move undoing
void undoMove(Board& board, uint64_t& positionHash, const MoveMemorial& memorial);
template <class TResultEvaluator> void undoMove(Board& board, uint64_t& positionHash, const MoveMemorial& memorial, TResultEvaluator& resultEvaluator)
{
    resultEvaluator.removeSingle();
    undoMove(board, positionHash, memorial);
}

void undoMove(Board& board, uint64_t& positionHash, const SimpleMoveMemorial& memorial);
template <class TResultEvaluator> void undoMove(Board& board, uint64_t& positionHash, const SimpleMoveMemorial& memorial, TResultEvaluator& resultEvaluator)
{
    resultEvaluator.removeSingle();
    undoMove(board, positionHash, memorial);
}

}  // namespace MoveApplier