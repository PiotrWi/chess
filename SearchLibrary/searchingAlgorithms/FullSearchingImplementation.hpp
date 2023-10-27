#pragma once

#include "BoardEngine.hpp"

#include <atomic>
#include <algorithm>
#include <cstring>
#include <vector>
#include <CachedEngines/FullCachedEngine.hpp>
#include <optional>

#include "core/NotationConversions.hpp"
#include "Constants.hpp"

#include "MoveOrdering.hpp"
#include "Statisctic.hpp"

static unsigned long long nodes;
static Statistic statistics;
constexpr short mateValue = 20000;

namespace
{

template <bool SaveMove>
int evaluateMax(BoardEngine& be,
                players::common::move_generators::FullCachedEngine& cachedEngine,
                unsigned char depth,
                short alfa,
                short beta);

ExtendedMove bestMove;
std::atomic_int interrupt_flag = 0;


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
                     short alfa,
                     short beta)
{
    ++nodes;
    if (be.isChecked())
    {
        return evaluateMax<false>(be, cachedEngine, 1, alfa, beta);
    }

    if (depth == 0) // terminate quiescence
    {
        auto validMoves = be.getMoveGeneratorV2(be.board.playerOnMove).getValidMoveCount(MoveGenerator::NotCheckedTag{});
        return cachedEngine.getEvaluationValue(be, validMoves);
    }
    auto mg = be.getMoveGeneratorV2(be.board.playerOnMove);
    auto validMoves = mg.getValidMoveCount(MoveGenerator::NotCheckedTag{});
    
    int stand_pat = evaluatePosition(be, cachedEngine, validMoves);
    if( stand_pat >= beta )
        return beta;
    if( alfa < stand_pat )
        alfa = stand_pat;

    auto memorial = be.getMemorial();
    int nextAlfa;

    auto moves = mg.generateBeatingMoves();
    auto orderedMoves = OnlyBeatingMovesSeeVersion(std::move(moves), be.board, be.lastMove, alfa-stand_pat);

    while (auto moveOpt = orderedMoves.get())
    {
        const auto& move = *moveOpt;

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
            orderedMoves.setMargin(alfa-stand_pat);
        }
    }

    return alfa;
}

template <bool SaveMove>
int evaluateMax(BoardEngine& be,
                players::common::move_generators::FullCachedEngine& cachedEngine,
                unsigned char depth,
                short alfa,
                short beta)
{
    ++nodes;

    if (interrupt_flag.load(std::memory_order_relaxed) == 0)
    {
        return alfa;
    }
    if (depth == 0)
    {
        if(be.are3Repeatitions())
        {
            return 0;
        }
        return quiescenceSearch(be, cachedEngine, 8/*Quinesence limit*/, alfa, beta);
    }

    auto mg = be.getMoveGeneratorV2(be.board.playerOnMove);
    auto validMoves = mg.getValidMoveCount();

    auto gameResult = be.getResult(validMoves);
    if(gameResult == Result::draw)
    {
        return 0;
    }
    if ((gameResult == Result::whiteWon) | (gameResult == Result::blackWon))
    {
        return -mateValue;
    }

    auto cache = cachedEngine.get(be);
    for (auto i = depth; i < MAX_DEPTH; ++i) {
        if (cache->previousEvaluations[i].visitedBefore)
        {
            if (cache->previousEvaluations[i].lowerValue >= beta)
                return beta;
            if (cache->previousEvaluations[i].higherValue <= alfa)
                return alfa;
            alfa = std::max(alfa, cache->previousEvaluations[i].lowerValue);
            beta = std::min(beta, cache->previousEvaluations[i].higherValue);
        }
    }

    auto memorial = be.getMemorial();
    ExtendedMove greatestMove;
    int nextAlfa;
    auto pvFound = false;

    auto orderedMoves = PreorderedMoves(
            be.board.playerOnMove,
            cache,
            depth,
            mg,
            be.lastMove,
            be.board
            );
    while (auto moveOpt = orderedMoves.get())
    {
        const auto& move = *moveOpt;
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
            cache->setBestMove(move, depth);
            cache->setLowerBound(nextAlfa, depth);

            setHistoryMove(be.board.playerOnMove, move, depth);

            if (SaveMove and interrupt_flag.load(std::memory_order_relaxed) != 0) bestMove = move;
            return beta;
        }
        if (nextAlfa > alfa)
        {
            pvFound = true;
            greatestMove = move;
            alfa = nextAlfa;
        }
    }
    if (not pvFound)
    {
        cache->setUpperBound(alfa, depth);
    }
    else
    {
        cache->setBestMove(greatestMove, depth);
        if (SaveMove and interrupt_flag.load(std::memory_order_relaxed) != 0) bestMove = greatestMove;
        cache->setLowerUpperBound(alfa, beta, depth);
    }
    return alfa;
}

}  // namespace

namespace full_search
{

// TODO: To do this correctly
// interrupt_flag as int is workaround.
// It shall solve a problem when searching ends faster than timer.

inline void interrupt()
{
    --interrupt_flag;
}

constexpr short InitialAlpha = -mateValue - 1;
constexpr short InitialBeta = mateValue + 1;

inline ExtendedMove evaluate(BoardEngine be,
              players::common::move_generators::FullCachedEngine& cachedEngine,
              unsigned char depth)
{
    ++interrupt_flag;
    short alfa = -mateValue;
    short beta = mateValue;
    clearHistoryMove();
    auto calculateStatistics = SinglePerrioadRaiiWrapper(statistics, nodes);

    evaluateMax<true>(be, cachedEngine, depth, alfa, beta);
    return bestMove;
}

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
    unsigned char maxDepth,
    void (*sendInfo)(int alpha, unsigned depth) = nullptr)
{
    bestMove = {};
    ++interrupt_flag;
    short alpha = InitialAlpha;
    short beta = InitialBeta;
    clearHistoryMove();
    auto calculateStatistics = SinglePerrioadRaiiWrapper(statistics, nodes);

    for (auto depth = 2u; depth <= maxDepth && depth < MAX_DEPTH && interrupt_flag; depth+=1)
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
            {
                std::cout << "Unstable" << std::endl;
                val = evaluateMax<true>(be, cachedEngine, depth, InitialAlpha, InitialBeta);
            }
        }

        if (sendInfo) { sendInfo(val, depth); }

        alpha = val - 30;
        beta = val + 30;
    }
    return bestMove;
}

}  // namespace full_search
