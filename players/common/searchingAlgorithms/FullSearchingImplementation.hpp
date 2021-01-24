#pragma once

#include <BoardEngine.hpp>

#include <algorithm>
#include <vector>
#include <common/CachedEngines/FullCachedEngine.hpp>

#include <publicIf/NotationConversions.hpp>

namespace
{

ExtendedMove bestMove;
unsigned history[2][64][64] = {};

void setHistoryMove(const NOTATION::COLOR::color player, ExtendedMove& move, unsigned char depth)
{
    if ((move.flags & ExtendedMove::beatingMask) == ExtendedMove::beatingMask)
    {
        auto index = 1;
        if (player == NOTATION::COLOR::color::white)
        {
            index = 0;
        }
        history[index][move.source][move.destination] +=
                depth * depth;
    }
}

class PreorderedMoves
{
public:
    PreorderedMoves(decltype(history[0])& historyMovesPerSinglePlayer,
                    players::common::move_generators::CacheFullEntity& ce,
                    unsigned char depth)
                    : historyMoves_(historyMovesPerSinglePlayer)
                    , ce_(ce)
                    , depth_(depth)
                    , index(0)
    {}
    ExtendedMove& get()
    {
        if (bestMoveAnalyzed== false)
        {
            auto firstNotBeatingIndex = std::find_if(ce_.precalculatedMoves.begin(),
                                                ce_.precalculatedMoves.end(),
                                                [](auto&& move) {
                        return !(move.flags & ExtendedMove::beatingMask);
            });
            nonBeatingIndex = firstNotBeatingIndex - ce_.precalculatedMoves.begin();

            bestMoveAnalyzed = true;
            for (auto candidateDepth = depth_; candidateDepth > 0; --candidateDepth)
            {
                if (ce_.previousBestMoves[candidateDepth].isSet)
                {
                    auto moveIt = std::find(ce_.precalculatedMoves.begin(),
                                            ce_.precalculatedMoves.end(),
                                            ce_.previousBestMoves[candidateDepth].move);
                    if (moveIt != ce_.precalculatedMoves.end()) // It could be cache miss.
                                                                // Then no move like this exist in valid moves vec.
                    {
                        std::swap(*moveIt, ce_.precalculatedMoves.front());
                        ++index;
                        if (moveIt != ce_.precalculatedMoves.begin()
                            && (moveIt->flags & ExtendedMove::beatingMask) != 0
                            && ((moveIt-1)->flags & ExtendedMove::beatingMask) != 0)
                        {
                            auto firstNotBeating = std::find_if(ce_.precalculatedMoves.begin(),
                                                             ce_.precalculatedMoves.end(),
                                                             [](auto&& move){
                                return !(move.flags & ExtendedMove::beatingMask);
                            });
                            if (firstNotBeating != ce_.precalculatedMoves.end())
                            {
                                std::swap(*moveIt, *firstNotBeating);
                            }
                        }
                        return ce_.previousBestMoves[candidateDepth].move;
                    }
                }
            }
        }

        if (index < nonBeatingIndex)
        {
            return ce_.precalculatedMoves[index++];
        }

        if (movesSortedByHistory == false)
        {
            movesSortedByHistory = true;
            std::sort(ce_.precalculatedMoves.begin() + index,
                      ce_.precalculatedMoves.end(),
                      [&](auto& lhs, auto& rhs) {
                return historyMoves_[lhs.source][lhs.destination] > historyMoves_[rhs.source][rhs.destination];
            });
        }
        return ce_.precalculatedMoves[index++];
    }
private:
    decltype(history[0])& historyMoves_;
    players::common::move_generators::CacheFullEntity& ce_;
    unsigned char depth_;
    bool bestMoveAnalyzed = false;
    bool movesSortedByHistory = false;
    int index;
    int nonBeatingIndex = 0;
};

int evaluatePosition(BoardEngine& be, players::common::move_generators::FullCachedEngine& moveGenerator)
{
    if(be.are3Repeatitions())
    {
        return 0;
    }

    return moveGenerator.getEvaluationValue(be) ;
}

template <bool SaveMove>
int evaluateMax(BoardEngine& be,
                players::common::move_generators::FullCachedEngine& cachedEngine,
                unsigned char depth,
                int alfa,
                int beta)
{
    if (depth == 0)
    {
        return evaluatePosition(be, cachedEngine);
    }

    auto cache = cachedEngine.get(be);

    auto gameResult = be.getResult(not cache.precalculatedMoves.empty());
    if(gameResult == Result::draw)
    {
        return 0;
    }
    if ((gameResult == Result::whiteWon) | (gameResult == Result::blackWon))
    {
        return -10000000;
    }

    if (cache.previousEvaluations[depth].visitedBefore)
    {
        if (cache.previousEvaluations[depth].lowerValue >= beta)
            return beta;
        if (cache.previousEvaluations[depth].higherValue <= alfa)
            return alfa;
        alfa = std::max(alfa, cache.previousEvaluations[depth].lowerValue);
        beta = std::min(beta, cache.previousEvaluations[depth].higherValue);
    }

    auto memorial = be.getMemorial();
    ExtendedMove greatestMove;
    auto nextAlfa = -1000000;
    auto pvFound = false;

    auto orderedMoves = PreorderedMoves(
            history[((be.board.playerOnMove == NOTATION::COLOR::color::white) ? 0 : 1)],
            cache,
            depth);
    for (auto i = 0u; i < cache.precalculatedMoves.size(); ++i)
    {
        auto move = orderedMoves.get();
        be.applyMove(move);
        if (pvFound)
        {
            nextAlfa = - evaluateMax<false>(be, cachedEngine, depth - 1, -alfa - 1, -alfa);
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
            if (SaveMove) bestMove = move;
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
    if (SaveMove) bestMove = greatestMove;
    return alfa;
}

}  // namespace

namespace full_search
{

Move evaluate(BoardEngine be,
              players::common::move_generators::FullCachedEngine& cachedEngine,
              unsigned char depth)
{
    int alfa = -10000000;
    int beta = 10000000;
    memset(history, 0, sizeof(history[0][0][0]) * 2 * 64 * 64);

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
Move evaluateIterative(BoardEngine be,
                       players::common::move_generators::FullCachedEngine& cachedEngine,
                       unsigned char maxDepth)
{
    int alpha = InitialAlpha;
    int beta = InitialBeta;
    memset(history, 0, sizeof(history[0][0][0]) * 2 * 64 * 64);

    for (auto depth = 2u; depth <= maxDepth; depth+=2)
    {
        auto val = evaluateMax<true>(be, cachedEngine, depth, alpha, beta);
        if (val <= alpha)
        {
            val = evaluateMax<true>(be, cachedEngine, depth, InitialAlpha, alpha + 1);
#ifdef ASSERTSON
            assert(val < alpha + 1);
#endif
        }
        else if (val >= beta)
        {
            val = evaluateMax<true>(be, cachedEngine, depth, beta - 1, InitialBeta);
#ifdef ASSERTSON
            assert(val > beta - 1);
#endif
        }
        alpha = val - 100;
        beta = val + 100;
    }
    return bestMove;
}

}  // namespace full_search
