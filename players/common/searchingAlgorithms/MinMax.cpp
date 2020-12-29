#include "MinMax.hpp"
#include <common/evaluators/MaterialEvaluator.hpp>

#include <iostream>

namespace
{

unsigned long nodes = 0;

signed char evaluateMin(BoardEngine &be, unsigned char depth, NOTATION::COLOR::color color);
signed char evaluateMax(BoardEngine& be, unsigned char depth, NOTATION::COLOR::color color);

signed char evaluateMin(BoardEngine &be, unsigned char depth, NOTATION::COLOR::color color)
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

    auto greatestValue = std::numeric_limits<signed char>::max();

    for (auto i = 0u; i < validMoves.size(); ++i)
    {
        auto undoHandle = be.applyUndoableMove(validMoves[i]);
        auto nodeVal = evaluateMax(be, depth - 1, color);
        be.undoMove(undoHandle);

        if (nodeVal < greatestValue)
        {
            greatestValue = nodeVal;
        }
    }
    return greatestValue;
}

signed char evaluateMax(BoardEngine& be, unsigned char depth, NOTATION::COLOR::color color)
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

    auto greatestValue = std::numeric_limits<signed char>::min();

    for (auto i = 0u; i < validMoves.size(); ++i)
    {
        auto undoHandle = be.applyUndoableMove(validMoves[i]);
        auto nodeVal = evaluateMin(be, depth - 1, color);
        be.undoMove(undoHandle);

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
    auto greatestValue = std::numeric_limits<signed char>::min();

    auto playerOnMove = be.board.playerOnMove;

    for (auto i = 0u; i < validMoves.size(); ++i)
    {
        auto undoHandle = be.applyUndoableMove(validMoves[i]);
        auto nodeVal = evaluateMin(be, depth - 1, playerOnMove);
        be.undoMove(undoHandle);

        if (nodeVal > greatestValue)
        {
            greatestValue = nodeVal;
            greatestMove = i;
        }
    }
    std::cout << "Calculated: " << nodes << std::endl;
    return validMoves[greatestMove];
}

}