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
        return -10000000;
    }

    auto oponentValidMoves = moveGenerator.generate(be, (be.board.playerOnMove + 1));

    return materialEvaluator::evaluate(be.board, color) + moveCountEvaluator::evaluate(validMoves.size(),
                                                                                       oponentValidMoves.size());
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
        return -10000000;
    }

    auto memorial = be.getMemorial();
    auto greatestMove = 0;
    for (auto i = 0u; i < validMoves.size(); ++i)
    {
        be.applyMove(validMoves[i]);
        auto nextAlfa = -evaluateMax(be, moveGenerator, depth - 1, color, -beta, -alfa);
        be.undoMove(memorial);

        if (beta <= nextAlfa)
        {
            moveGenerator.setKillerMove(be, i);
            return beta;
        }
        if (nextAlfa > alfa)
        {
            greatestMove = i;
            alfa = nextAlfa;
        }
    }
    moveGenerator.setKillerMove(be, greatestMove);
    return alfa;
}

}  // namespace

namespace alfaBeta
{

template <typename TMoveGenerator>
Move evaluate(BoardEngine be, TMoveGenerator& moveGenerator, unsigned char depth)
{
    auto validMoves = moveGenerator.generate(be);
    auto greatestMove = 0u;

    int alfa = -10000000;
    int beta = 10000000;

    auto playerOnMove = be.board.playerOnMove;

    auto memorial = be.getMemorial();
    for (auto i = 0u; i < validMoves.size(); ++i)
    {
        be.applyMove(validMoves[i]);                                           //-inf   +inf
        auto nextAlfa = -evaluateMax(be, moveGenerator, depth - 1, playerOnMove, -beta, -alfa);
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
