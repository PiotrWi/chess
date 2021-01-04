#pragma once

#include "publicIf/Board.hpp"

class ResultEvaluator;

namespace MoveApplier
{

/* To check what is faster.
 * Full copy of board or restoring board by MoveMemorial. */

struct MoveMemorial
{
    decltype(Board::lastMove) lastMove;
    bool wasCasling;
    bool enPasant;
    unsigned char sourceField;
    unsigned char sourceVal;
    unsigned char targetField;
    unsigned char targetVal;
};

struct SimpleMoveMemorial
{
    Board previousBoard;
};

void applyMove(Board&, const Move&);
MoveMemorial applyTmpMove(Board&, const Move&);
SimpleMoveMemorial applyTmpMoveSimple(Board&, const Move&);

template <class TResultEvaluator> void applyMove(Board& board, const Move& move, TResultEvaluator& resultEvaluator)
{
    applyMove(board, move);
    resultEvaluator.storeBoard(board);
}
template <class TResultEvaluator> MoveMemorial applyTmpMove(Board& board, const Move& move, TResultEvaluator& resultEvaluator)
{
    auto memorial = applyTmpMove(board, move);
    resultEvaluator.storeBoard(board);
    return memorial;
}

template <class TResultEvaluator> SimpleMoveMemorial applyTmpMoveSimple(Board& board,
        const Move& move, TResultEvaluator& resultEvaluator)
{
    auto memorial = applyTmpMoveSimple(board, move);
    resultEvaluator.storeBoard(board);
    return memorial;
}

// TODO: There is something wrong with move undoing
void undoMove(Board& board, const MoveMemorial& memorial);
template <class TResultEvaluator> void undoMove(Board& board, const MoveMemorial& memorial, TResultEvaluator& resultEvaluator)
{
    resultEvaluator.removeSingle();
    undoMove(board, memorial);
}

void undoMove(Board& board, const SimpleMoveMemorial& memorial);
template <class TResultEvaluator> void undoMove(Board& board, const SimpleMoveMemorial& memorial, TResultEvaluator& resultEvaluator)
{
    resultEvaluator.removeSingle();
    undoMove(board, memorial);
}

}  // namespace MoveApplier