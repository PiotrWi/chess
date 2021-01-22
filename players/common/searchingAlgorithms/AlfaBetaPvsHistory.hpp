#pragma once

#pragma once

#include <BoardEngine.hpp>

#include <algorithm>
#include <vector>

#include <common/evaluators/MaterialEvaluator.hpp>
#include <common/evaluators/MoveCountEvaluator.hpp>
#include <publicIf/NotationConversions.hpp>

namespace
{

ExtendedMove bestMove;

unsigned history[2][64][64] = {};

void sortByHistory(NOTATION::COLOR::color c, std::vector<ExtendedMove>& moves)
{
    auto index = 1;
    if (c == NOTATION::COLOR::color::white)
    {
        index = 0;
    }
    std::sort(moves.begin(), moves.end(), [&](auto& lhs, auto& rhs)
    {
        return history[index][lhs.source][lhs.destination] > history[index][rhs.source][rhs.destination];
    });
}

template <typename TMoveGenerator>
int evaluatePosition(BoardEngine& be,
                     TMoveGenerator& moveGenerator)
{
    if(be.are3Repeatitions())
    {
        return 0;
    }

    return moveGenerator.getEvaluationValue(be) ;
}

template <typename TMoveGenerator, bool SaveMove>
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
    auto nextAlfa = -1000000;
    auto pvFound = false;

    sortByHistory(be.board.playerOnMove, validMoves);
    for (auto i = 0u; i < validMoves.size(); ++i)
    {
        be.applyMove(validMoves[i]);
        if (pvFound)
        {
            nextAlfa = - evaluateMax<TMoveGenerator, false>(be, moveGenerator, depth - 1, -alfa - 1, -alfa);
            if (nextAlfa > alfa && nextAlfa < beta)
            {
                nextAlfa = -evaluateMax<TMoveGenerator, false>(be, moveGenerator, depth - 1, -beta, -alfa);
            }
        }
        else
        {
            nextAlfa = -evaluateMax<TMoveGenerator, false>(be, moveGenerator, depth - 1, -beta, -alfa);
        }
        be.undoMove(memorial);

        if (beta <= nextAlfa)
        {
            if ((validMoves[i].flags & ExtendedMove::beatingMask) == ExtendedMove::beatingMask)
            {

                auto index = 1;
                if (be.board.playerOnMove == NOTATION::COLOR::color::white)
                {
                    index = 0;
                }
                history[index][validMoves[i].source][validMoves[i].destination] += depth*depth;
            }
            moveGenerator.setKillerMove(be, i, depth);
            if (SaveMove) bestMove = validMoves[i];
            return beta;
        }
        if (nextAlfa > alfa)
        {
            pvFound = true;
            greatestMove = i;
            alfa = nextAlfa;
        }
    }
    moveGenerator.setKillerMove(be, greatestMove, depth);
    if (SaveMove) bestMove = validMoves[greatestMove];
    return alfa;
}

}  // namespace

namespace alfaBetaPvsHistory
{

template <typename TMoveGenerator>
Move evaluate(BoardEngine be, TMoveGenerator& moveGenerator, unsigned char depth)
{
    memset(history, 0, sizeof(history[0][0][0]) * 2 * 64 * 64);

    int alfa = -10000000;
    int beta = 10000000;

    evaluateMax<TMoveGenerator, true>(be, moveGenerator, depth, alfa, beta);
    return bestMove;
}



constexpr auto mateValue = 10000000;
constexpr auto InitialAlpha = -mateValue - 1;
constexpr auto InitialBeta = mateValue + 1;

/**
 * @brief
 * NOTE: Currently only even depth are supported. Horizont effect shall be lower for these.
 */
template <typename TMoveGenerator>
Move evaluateIterative(BoardEngine be, TMoveGenerator& moveGenerator, unsigned char maxDepth)
{
    memset(history, 0, sizeof(history[0][0][0]) * 2 * 64 * 64);

    int alpha = InitialAlpha;
    int beta = InitialBeta;

    for (auto depth = 2u; depth <= maxDepth; depth+=2)
    {
        auto val = evaluateMax<TMoveGenerator, true>(be, moveGenerator, depth, alpha, beta);
        if (val <= alpha)
        {
            val = evaluateMax<TMoveGenerator, true>(be, moveGenerator, depth, InitialAlpha, alpha + 1);
#ifdef ASSERTSON
            assert(val < alpha + 1);
#endif
        }
        else if (val >= beta)
        {
            val = evaluateMax<TMoveGenerator, true>(be, moveGenerator, depth, beta - 1, InitialBeta);
#ifdef ASSERTSON
            assert(val > beta - 1);
#endif
        }
        alpha = val - 100;
        beta = val + 100;
    }
    return bestMove;
}

}  // namespace alfaBeta
