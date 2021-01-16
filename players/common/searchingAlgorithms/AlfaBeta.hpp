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
int evaluateMax(BoardEngine& be,
                TMoveGenerator& moveGenerator,
                unsigned char depth,
                int alfa,
                int beta);

template <typename TMoveGenerator>
int evaluatePosition(BoardEngine& be,
                     TMoveGenerator& moveGenerator)
{
    if(be.are3Repeatitions())
    {
        return 0;
    }

    return moveGenerator.getEvaluationValue(be);
}

template <typename TMoveGenerator>
int evaluateMax(BoardEngine& be,
                TMoveGenerator& moveGenerator,
                unsigned char depth,
                int alfa,
                int beta)
{
    if (depth == 0)
    {
        return evaluatePosition(be, moveGenerator);
    }

    auto validMoves = moveGenerator.generate(be, depth);
    auto gameResult = be.getResult(not validMoves.empty());
    if(gameResult == Result::draw)
    {
        return 0;
    }
    if ((gameResult == Result::whiteWon) | (gameResult == Result::blackWon))
    {
        return -10000000;
    }

    auto memorial = be.getMemorial();
    auto greatestMove = 0;
    for (auto i = 0u; i < validMoves.size(); ++i)
    {
        be.applyMove(validMoves[i]);
        auto nextAlfa = -evaluateMax(be, moveGenerator, depth - 1, -beta, -alfa);
        be.undoMove(memorial);

        if (beta <= nextAlfa)
        {
            moveGenerator.setKillerMove(be, i, depth);
            return beta;
        }
        if (nextAlfa > alfa)
        {
            greatestMove = i;
            alfa = nextAlfa;
        }
    }
    moveGenerator.setKillerMove(be, greatestMove, depth);
    return alfa;
}

}  // namespace

namespace alfaBeta
{

template <typename TMoveGenerator>
Move evaluate(BoardEngine be, TMoveGenerator& moveGenerator, unsigned char depth)
{
    auto validMoves = moveGenerator.generate(be, depth);
    auto greatestMove = 0u;

    int alfa = -10000000;
    int beta = 10000000;

    auto memorial = be.getMemorial();
    for (auto i = 0u; i < validMoves.size(); ++i)
    {
        be.applyMove(validMoves[i]);
        auto nextAlfa = -evaluateMax(be, moveGenerator, depth - 1, -beta, -alfa);
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
