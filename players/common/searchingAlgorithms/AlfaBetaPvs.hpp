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
    auto nextAlfa = -1000000;
    auto pvFound = false;

    for (auto i = 0u; i < validMoves.size(); ++i)
    {
        be.applyMove(validMoves[i]);
        if (pvFound)
        {
            nextAlfa = - evaluateMax(be, moveGenerator, depth - 1, color, -alfa - 1, -alfa);
            if (nextAlfa > alfa && nextAlfa < beta)
            {
                nextAlfa = -evaluateMax(be, moveGenerator, depth - 1, color, -beta, -alfa);
            }
        }
        else
        {
            nextAlfa = -evaluateMax(be, moveGenerator, depth - 1, color, -beta, -alfa);
        }
        be.undoMove(memorial);

        if (beta <= nextAlfa)
        {
            moveGenerator.setKillerMove(be, i);
            return beta;
        }
        if (nextAlfa > alfa)
        {
            pvFound = true;
            greatestMove = i;
            alfa = nextAlfa;
        }
    }
    moveGenerator.setKillerMove(be, greatestMove);
    return alfa;
}

}  // namespace

namespace alfaBetaPvs
{

template <typename TMoveGenerator>
Move evaluate(BoardEngine be, TMoveGenerator& moveGenerator, unsigned char depth)
{
    auto validMoves = moveGenerator.generate(be);
    auto greatestMove = 0u;

    int alfa = -10000000;
    int beta = 10000000;

    auto playerOnMove = be.board.playerOnMove;

    auto pvFound = false;
    auto nextAlfa = -1000000;
    auto memorial = be.getMemorial();
    for (auto i = 0u; i < validMoves.size(); ++i)
    {
        be.applyMove(validMoves[i]);
        if (pvFound)
        {
            nextAlfa = - evaluateMax(be, moveGenerator, depth - 1, playerOnMove, -alfa - 1, -alfa);
            if (nextAlfa > alfa && nextAlfa < beta)
            {
                nextAlfa = -evaluateMax(be, moveGenerator, depth - 1, playerOnMove, -beta, -alfa);
            }
        }
        else
        {
            nextAlfa = -evaluateMax(be, moveGenerator, depth - 1, playerOnMove, -beta, -alfa);
        }
        be.undoMove(memorial);

        if (nextAlfa > alfa)
        {
            pvFound = true;
            alfa = nextAlfa;
            greatestMove = i;
        }
    }
    return validMoves[greatestMove];
}

}  // namespace alfaBeta
