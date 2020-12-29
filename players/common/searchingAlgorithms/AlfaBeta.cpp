#include "AlfaBeta.hpp"

#include <common/evaluators/MaterialEvaluator.hpp>

namespace
{

signed char evaluateMin(BoardEngine& be,
                        unsigned char depth,
                        NOTATION::COLOR::color color,
                        signed char alfa,
                        signed char beta);
signed char evaluateMax(BoardEngine& be,
                        unsigned char depth,
                        NOTATION::COLOR::color color,
                        signed char alfa,
                        signed char beta);

signed char evaluateMin(BoardEngine& be,
                        unsigned char depth,
                        NOTATION::COLOR::color color,
                        signed char alfa,
                        signed char beta)
{
    auto validMoves = be.generateMoves();
    auto gameResult = be.getResult(not validMoves.empty());
    if(gameResult == Result::draw)
    {
        return 0;
    }
    if ((gameResult == Result::whiteWon) | (gameResult == Result::blackWon))
    {
        return std::numeric_limits<signed char>::max();
    }
    if (depth == 0)
    {
        return materialEvaluator::evaluate(be.board, color);
    }

    for (auto i = 0u; i < validMoves.size(); ++i)
    {
        auto undoHandle = be.applyUndoableMove(validMoves[i]);
        auto nextBeta = evaluateMax(be, depth - 1, color, alfa, beta);
        be.undoMove(undoHandle);

        if (nextBeta < beta)
        {
            beta = nextBeta;
        }
        if (alfa >= beta)
        {
            return beta;
        }
    }
    return beta;
}

signed char evaluateMax(BoardEngine& be,
                        unsigned char depth,
                        NOTATION::COLOR::color color,
                        signed char alfa,
                        signed char beta)
{
    auto validMoves = be.generateMoves();
    auto gameResult = be.getResult(not validMoves.empty());
    if(gameResult == Result::draw)
    {
        return 0;
    }
    if ((gameResult == Result::whiteWon) | (gameResult == Result::blackWon))
    {
        return std::numeric_limits<signed char>::min();
    }
    if (depth == 0)
    {
        return materialEvaluator::evaluate(be.board, color);
    }

    for (auto i = 0u; i < validMoves.size(); ++i)
    {
        auto undoHandle = be.applyUndoableMove(validMoves[i]);
        auto nextAlfa = evaluateMin(be, depth - 1, color, alfa, beta);
        be.undoMove(undoHandle);

        if (nextAlfa > alfa)
        {
            alfa = nextAlfa;
        }
        if (alfa >= beta)
        {
            return alfa;
        }
    }
    return alfa;
}

}  // namespace

namespace alfaBeta
{

Move evaluate(BoardEngine be, unsigned char depth)
{
    auto greatestMove = 0u;

    signed char alfa = std::numeric_limits<signed char>::min();
    signed char beta = std::numeric_limits<signed char>::max();

    auto validMoves = be.generateMoves();
    auto playerOnMove = be.board.playerOnMove;

    for (auto i = 0u; i < validMoves.size(); ++i)
    {
        auto undoHandle = be.applyUndoableMove(validMoves[i]);
        auto nextAlfa = evaluateMin(be, depth - 1, playerOnMove, alfa, beta);
        be.undoMove(undoHandle);

        if (nextAlfa > alfa)
        {
            alfa = nextAlfa;
            greatestMove = i;
        }
        if (alfa >= beta)
        {
            return validMoves[greatestMove];
        }
    }
    return validMoves[greatestMove];
}

}  // namespace alfaBeta
