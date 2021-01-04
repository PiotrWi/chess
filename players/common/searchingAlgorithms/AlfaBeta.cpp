#include "AlfaBeta.hpp"

#include <algorithm>
#include <vector>

#include <common/evaluators/MaterialEvaluator.hpp>
#include <common/evaluators/MoveCountEvaluator.hpp>

namespace
{

int evaluateMin(BoardEngine& be,
                        unsigned char depth,
                        NOTATION::COLOR::color color,
                        int alfa,
                        int beta);
int evaluateMax(BoardEngine& be,
                        unsigned char depth,
                        NOTATION::COLOR::color color,
                        int alfa,
                        int beta);

int evaluateMin(BoardEngine& be,
                        unsigned char depth,
                        NOTATION::COLOR::color color,
                        int alfa,
                        int beta)
{
    auto validMoves = be.generateMoves();
    auto gameResult = be.getResult(not validMoves.empty());
    if(gameResult == Result::draw)
    {
        return 0;
    }
    if ((gameResult == Result::whiteWon) | (gameResult == Result::blackWon))
    {
        return std::numeric_limits<int>::max();
    }
    if (depth == 0)
    {
        return materialEvaluator::evaluate(be.board, color);
    }

    auto greatestValue = std::numeric_limits<int>::max();
    for (auto i = 0u; i < validMoves.size(); ++i)
    {
        auto memorial = be.applyUndoableSimpleMove(validMoves[i]);
        auto nextBeta = evaluateMax(be, depth - 1, color, alfa, beta);
        be.undoMove(memorial);

        greatestValue = std::min(greatestValue, nextBeta);
        beta = std::min(beta, nextBeta);

        if (beta <= alfa)
            break;
    }
    return greatestValue;
}

int evaluateMax(BoardEngine& be,
                        unsigned char depth,
                        NOTATION::COLOR::color color,
                        int alfa,
                        int beta)
{
    auto validMoves = be.generateMoves();
    auto gameResult = be.getResult(not validMoves.empty());
    if(gameResult == Result::draw)
    {
        return 0;
    }
    if ((gameResult == Result::whiteWon) | (gameResult == Result::blackWon))
    {
        return std::numeric_limits<int>::min();
    }
    if (depth == 0)
    {
        return materialEvaluator::evaluate(be.board, color);
    }

    auto greatestValue = std::numeric_limits<int>::min();
    for (auto i = 0u; i < validMoves.size(); ++i)
    {
        auto memorial = be.applyUndoableSimpleMove(validMoves[i]);
        auto nextAlfa = evaluateMin(be, depth - 1, color, alfa, beta);
        be.undoMove(memorial);

        greatestValue = std::max(greatestValue, nextAlfa);
        alfa = std::max(alfa, nextAlfa);

        if (beta <= alfa)
            break;
    }
    return greatestValue;
}

}  // namespace

namespace alfaBeta
{

Move evaluate(BoardEngine be, unsigned char depth)
{
    auto validMoves = be.generateMoves();
    auto greatestMove = 0u;

    int alfa = std::numeric_limits<int>::min();
    int beta = std::numeric_limits<int>::max();

    auto playerOnMove = be.board.playerOnMove;

    for (auto i = 0u; i < validMoves.size(); ++i)
    {
        auto memorial = be.applyUndoableSimpleMove(validMoves[i]);
        auto nextAlfa = evaluateMin(be, depth - 1, playerOnMove, alfa, beta);
        be.undoMove(memorial);

        if (nextAlfa > alfa)
        {
            alfa = nextAlfa;
            greatestMove = i;
        }
    }
    return validMoves[greatestMove];
}

}  // namespace alfaBeta
