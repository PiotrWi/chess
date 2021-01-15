#pragma once

#include <BoardEngine.hpp>

#include <algorithm>
#include <vector>

#include <common/evaluators/MaterialEvaluator.hpp>
#include <common/evaluators/MoveCountEvaluator.hpp>
#include <publicIf/NotationConversions.hpp>

namespace
{

template <typename TMoveGenerator>
int evaluateMin(BoardEngine& be,
                TMoveGenerator& moveGenerator,
                unsigned char depth,
                NOTATION::COLOR::color color,
                int alfa,
                int beta);

template <typename TMoveGenerator>
int evaluateMax(BoardEngine& be,
                TMoveGenerator& moveGenerator,
                unsigned char depth,
                NOTATION::COLOR::color color,
                int alfa,
                int beta);

template <typename TMoveGenerator>
int evaluatePosition(BoardEngine& be,
                     TMoveGenerator& moveGenerator,
                     NOTATION::COLOR::color color)
{
    auto validMoves = moveGenerator.generate(be);
    auto gameResult = be.getResult(not validMoves.empty());
    if(gameResult == Result::draw)
    {
        return 0;
    }

    if ((gameResult == Result::whiteWon) | (gameResult == Result::blackWon))
    {
        return std::numeric_limits<int>::max();
    }

    auto oponentValidMoves = moveGenerator.generate(be, (be.board.playerOnMove + 1));

    return materialEvaluator::evaluate(be.board, color) + moveCountEvaluator::evaluate(validMoves.size(),
                                                                                       oponentValidMoves.size());
}

template <typename TMoveGenerator>
int evaluateMin(BoardEngine& be,
                TMoveGenerator& moveGenerator,
                unsigned char depth,
                NOTATION::COLOR::color color,
                int alfa,
                int beta)
{
    if (depth == 0)
    {
        return -evaluatePosition(be, moveGenerator, color);
    }

    auto validMoves = moveGenerator.generate(be);
    auto gameResult = be.getResult(not validMoves.empty());
    if(gameResult == Result::draw)
    {
        return 0;
    }
    if ((gameResult == Result::whiteWon) | (gameResult == Result::blackWon))
    {
        return std::numeric_limits<int>::max();
    }

    auto greatestValue = std::numeric_limits<int>::max();
    auto memorial = be.getMemorial();
    auto greatestMove = 0;
    for (auto i = 0u; i < validMoves.size(); ++i)
    {
        be.applyMove(validMoves[i]);
        auto nextBeta = evaluateMax(be, moveGenerator, depth - 1, color, alfa, beta);
        be.undoMove(memorial);

        if (nextBeta < greatestValue)
        {
            greatestValue = nextBeta;
            greatestMove = i;
        }
        beta = std::min(beta, nextBeta);

        if (beta <= alfa)
        {
            moveGenerator.setKillerMove(be, i);
            return greatestValue;
        }
    }
    moveGenerator.setKillerMove(be, greatestMove);
    return greatestValue;
}

template <typename TMoveGenerator>
int evaluateMax(BoardEngine& be,
                TMoveGenerator& moveGenerator,
                unsigned char depth,
                NOTATION::COLOR::color color,
                int alfa,
                int beta)
{
    if (depth == 0)
    {
        return evaluatePosition(be, moveGenerator, color);
    }

    auto validMoves = moveGenerator.generate(be);
    auto gameResult = be.getResult(not validMoves.empty());
    if(gameResult == Result::draw)
    {
        return 0;
    }
    if ((gameResult == Result::whiteWon) | (gameResult == Result::blackWon))
    {
        return std::numeric_limits<int>::min();
    }

    auto memorial = be.getMemorial();
    auto greatestValue = std::numeric_limits<int>::min();
    auto greatestMove = 0;
    for (auto i = 0u; i < validMoves.size(); ++i)
    {
        be.applyMove(validMoves[i]);
        auto nextAlfa = evaluateMin(be, moveGenerator, depth - 1, color, alfa, beta);
        be.undoMove(memorial);

        if (nextAlfa > greatestValue)
        {
            greatestValue = nextAlfa;
            greatestMove = i;
        }
        alfa = std::max(alfa, nextAlfa);

        if (beta <= alfa)
        {
            moveGenerator.setKillerMove(be, i);
            return greatestValue;
        }
    }
    moveGenerator.setKillerMove(be, greatestMove);
    return greatestValue;
}

}  // namespace

namespace alfaBeta
{

template <typename TMoveGenerator>
Move evaluate(BoardEngine be, TMoveGenerator& moveGenerator, unsigned char depth)
{
    auto validMoves = moveGenerator.generate(be);
    auto greatestMove = 0u;

    int alfa = std::numeric_limits<int>::min();
    int beta = std::numeric_limits<int>::max();

    auto playerOnMove = be.board.playerOnMove;

    auto memorial = be.getMemorial();
    for (auto i = 0u; i < validMoves.size(); ++i)
    {
        be.applyMove(validMoves[i]);
        auto nextAlfa = evaluateMin(be, moveGenerator, depth - 1, playerOnMove, alfa, beta);
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
