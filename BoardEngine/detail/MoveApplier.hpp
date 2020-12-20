#pragma once

#include "publicIf/Board.hpp"

class ResultEvaluator;

namespace MoveApplier
{

void applyMove(Board&, const Move&);
template <class TResultEvaluator> void applyMove(Board& board, const Move& move, TResultEvaluator& resultEvaluator)
{
    applyMove(board, move);
    resultEvaluator.storeBoard(board);
}

}  // namespace MoveApplier