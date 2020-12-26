#pragma once

#include "publicIf/Board.hpp"

class ResultEvaluator;

namespace MoveApplier
{

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

void applyMove(Board&, const Move&);
MoveMemorial applyTmpMove(Board&, const Move&);

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

void undoMove(Board& board, const MoveMemorial& memorial);
template <class TResultEvaluator> void undoMove(Board& board, const MoveMemorial& memorial, TResultEvaluator& resultEvaluator)
{
    resultEvaluator.removeSingle();
    undoMove(board, memorial);
}

}  // namespace MoveApplier