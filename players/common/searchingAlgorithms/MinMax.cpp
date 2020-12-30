#include "MinMax.hpp"


#include <common/evaluators/MaterialEvaluator.hpp>

namespace
{

int evaluateMin(BoardEngine& be, unsigned char depth, NOTATION::COLOR::color color);
int evaluateMax(BoardEngine& be, unsigned char depth, NOTATION::COLOR::color color);

int evaluateMin(BoardEngine& be, unsigned char depth, NOTATION::COLOR::color color)
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
        auto nodeVal = evaluateMax(be, depth - 1, color);
        be.undoMove(memorial);

        if (nodeVal < greatestValue)
        {
            greatestValue = nodeVal;
        }
    }
    return greatestValue;
}

int evaluateMax(BoardEngine& be, unsigned char depth, NOTATION::COLOR::color color)
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
        auto nodeVal = evaluateMin(be, depth - 1, color);
        be.undoMove(memorial);

        if (nodeVal > greatestValue)
        {
            greatestValue = nodeVal;
        }
    }
    return greatestValue;
}

}  // namespace

namespace minMax
{

Move evaluate(BoardEngine be, unsigned char depth)
{
    auto validMoves = be.generateMoves();
    auto greatestMove = 0u;
    auto greatestValue = std::numeric_limits<int>::min();

    auto playerOnMove = be.board.playerOnMove;

    for (auto i = 0u; i < validMoves.size(); ++i)
    {
        auto memorial = be.applyUndoableSimpleMove(validMoves[i]);
        auto nodeVal = evaluateMin(be, depth - 1, playerOnMove);
        be.undoMove(memorial);

        if (nodeVal > greatestValue)
        {
            greatestValue = nodeVal;
            greatestMove = i;
        }
    }
    return validMoves[greatestMove];
}

}