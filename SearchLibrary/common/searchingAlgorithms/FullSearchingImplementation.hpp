#pragma once

#include <BoardEngine.hpp>

#include <atomic>
#include <algorithm>
#include <cstring>
#include <vector>
#include <common/CachedEngines/FullCachedEngine.hpp>

#include <publicIf/NotationConversions.hpp>
#include <common/Constants.hpp>

#include "MoveOrdering.hpp"
#include "Statisctic.hpp"

static unsigned long long nodes;
static Statistic statistics;

namespace
{

template <bool SaveMove>
int evaluateMax(BoardEngine& be,
                players::common::move_generators::FullCachedEngine& cachedEngine,
                unsigned char depth,
                int alfa,
                int beta);

ExtendedMove bestMove;
std::atomic_bool interrupt_flag = false;


int evaluatePosition(BoardEngine& be, players::common::move_generators::FullCachedEngine& moveGenerator, unsigned int validMoves)
{
    if(be.are3Repeatitions())
    {
        return 0;
    }

    return moveGenerator.getEvaluationValue(be, validMoves);
}

int quiescenceSearch(BoardEngine& be,
                     players::common::move_generators::FullCachedEngine& cachedEngine,
                     unsigned char depth,
                     int alfa,
                     int beta)
{
    ++nodes;
    if (be.isChecked())
    {
        return evaluateMax<false>(be, cachedEngine, 1, alfa, beta);
    }

    if (depth == 0) // terminate quiescence
    {
        return cachedEngine.getEvaluationValue(be, be.generateValidMoveCount());
    }

    auto moves = be.generateMoves();
    auto validMoves = moves.size();

    auto orderedMoves = OnlyBeatingMoves(std::move(moves));
    auto stablePosition = orderedMoves.size() == 0;
    if (stablePosition)
    {
        return cachedEngine.getEvaluationValue(be, validMoves) ;
    }

    int stand_pat = evaluatePosition(be, cachedEngine, validMoves);
    if( stand_pat >= beta )
        return beta;
    if( alfa < stand_pat )
        alfa = stand_pat;

    auto memorial = be.getMemorial();
    auto nextAlfa = -1000000;

    for (auto i = 0u; i < orderedMoves.size(); ++i)
    {
        auto move = orderedMoves.get();
        be.applyMove(move);
        nextAlfa = -quiescenceSearch(be, cachedEngine, depth - 1, -beta, -alfa);
        be.undoMove(memorial);
        if (beta <= nextAlfa)
        {
            return beta;
        }
        if (nextAlfa > alfa)
        {
            alfa = nextAlfa;
        }
    }
    return alfa;
}

template <bool SaveMove>
int evaluateMax(BoardEngine& be,
                players::common::move_generators::FullCachedEngine& cachedEngine,
                unsigned char depth,
                int alfa,
                int beta)
{
    ++nodes;
    if (interrupt_flag.load(std::memory_order_relaxed))
    {
        return alfa;
    }
    if (depth == 0)
    {
        if(be.are3Repeatitions())
        {
            return 0;
        }
        return quiescenceSearch(be, cachedEngine, 6/*Quinesence limit*/, alfa, beta);
    }

    auto cache = cachedEngine.get(be);
    auto moves = be.generateMoves();

    auto gameResult = be.getResult(not moves.empty());
    if(gameResult == Result::draw)
    {
        return 0;
    }
    if ((gameResult == Result::whiteWon) | (gameResult == Result::blackWon))
    {
        return -10000000;
    }

    for (auto i = depth; i < MAX_DEPTH; ++i) {
        if (cache->previousEvaluations[i].visitedBefore)
        {
            if (cache->previousEvaluations[i].lowerValue >= beta)
                return beta;
            if (cache->previousEvaluations[i].higherValue <= alfa)
                return alfa;
            alfa = std::max(alfa, cache->previousEvaluations[i].lowerValue);
            beta = std::min(beta, cache->previousEvaluations[i].higherValue);
            break;
        }
    }

    auto memorial = be.getMemorial();
    ExtendedMove greatestMove;
    auto nextAlfa = -1000000;
    auto pvFound = false;

    auto orderedMoves = PreorderedMoves(
            be.board.playerOnMove,
            cache,
            depth,
            std::move(moves));
    for (auto i = 0u; i < orderedMoves.size(); ++i)
    {
        auto move = orderedMoves.get();
        be.applyMove(move);
        if (pvFound)
        {
            nextAlfa = -evaluateMax<false>(be, cachedEngine, depth - 1, -alfa - 1, -alfa);
            if (nextAlfa > alfa && nextAlfa < beta)
            {
                nextAlfa = -evaluateMax<false>(be, cachedEngine, depth - 1, -beta, -alfa);
            }
        }
        else
        {
            nextAlfa = -evaluateMax<false>(be, cachedEngine, depth - 1, -beta, -alfa);
        }
        be.undoMove(memorial);

        if (beta <= nextAlfa)
        {
            cachedEngine.setBestMove(be, move, depth);
            cachedEngine.setLowerBound(be, nextAlfa, depth);

            setHistoryMove(be.board.playerOnMove, move, depth);

            if (SaveMove and interrupt_flag.load(std::memory_order_relaxed) == false) bestMove = move;
            return beta;
        }
        if (nextAlfa > alfa)
        {
            pvFound = true;
            greatestMove = move;
            alfa = nextAlfa;
        }
    }
    cachedEngine.setBestMove(be, greatestMove, depth);
    if (not pvFound)
    {
        cachedEngine.setUpperBound(be, alfa, depth);
    }
    else
    {
        cachedEngine.setLowerUpperBound(be, alfa, beta, depth);
    }
    if (SaveMove and interrupt_flag.load(std::memory_order_relaxed) == false) bestMove = greatestMove;
    return alfa;
}

}  // namespace

namespace full_search
{

inline void interrupt()
{
    interrupt_flag = true;
}

inline ExtendedMove evaluate(BoardEngine be,
              players::common::move_generators::FullCachedEngine& cachedEngine,
              unsigned char depth)
{
    interrupt_flag = false;
    int alfa = -10000000;
    int beta = 10000000;
    clearHistoryMove();
    auto calculateStatistics = SinglePerrioadRaiiWrapper(statistics, nodes);

    evaluateMax<true>(be, cachedEngine, depth, alfa, beta);
    return bestMove;
}

constexpr auto mateValue = 10000000;
constexpr auto InitialAlpha = -mateValue - 1;
constexpr auto InitialBeta = mateValue + 1;

/**
 * @brief
 * It contains implementation of:
 *  - iterative deeping
 *  - aspiration window
 *
 * NOTE: Iterative searching for the best move.
 */
    inline ExtendedMove evaluateIterative(BoardEngine be,
                       players::common::move_generators::FullCachedEngine& cachedEngine,
                       unsigned char maxDepth)
{
    bestMove = {};
    interrupt_flag = false;
    int alpha = InitialAlpha;
    int beta = InitialBeta;
    clearHistoryMove();
    auto calculateStatistics = SinglePerrioadRaiiWrapper(statistics, nodes);

    for (auto depth = 2u; depth <= maxDepth && depth < MAX_DEPTH; depth+=1)
    {
        auto val = evaluateMax<true>(be, cachedEngine, depth, alpha, beta);
        if (val <= alpha)
        {
            val = evaluateMax<true>(be, cachedEngine, depth, InitialAlpha, alpha + 1);
            if(val >= beta)
            {
                std::cout << "Unstable" << std::endl;
                val = evaluateMax<true>(be, cachedEngine, depth, InitialAlpha, InitialBeta);
            }
        }
        else if (val >= beta)
        {
            val = evaluateMax<true>(be, cachedEngine, depth, beta - 1, InitialBeta);
            if (val <= alpha)
                std::cout << "Unstable" << std::endl;
            val = evaluateMax<true>(be, cachedEngine, depth, InitialAlpha, InitialBeta);
        }
        alpha = val - 30;
        beta = val + 30;
    }
    return bestMove;
}

}  // namespace full_search
