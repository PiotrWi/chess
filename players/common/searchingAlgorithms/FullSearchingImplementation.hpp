#pragma once

#include <BoardEngine.hpp>

#include <atomic>

#include <algorithm>
#include <cstring>
#include <vector>
#include <common/CachedEngines/FullCachedEngine.hpp>

#include <publicIf/NotationConversions.hpp>
#include <common/Constants.hpp>

namespace
{

ExtendedMove bestMove;
std::atomic_bool interrupt_flag = false;

unsigned history[2][64][64] = {};

class MVVLVA_Comparator // Most valuable victim less valuable aggressor
{
public:
    static bool compare(const ExtendedMove& lhs, const ExtendedMove& rhs)
    {
        auto lhsVal = (lhs.flags & ExtendedMove::promotionMask) ? FIGURES_VALUE::QUEEN : 0;
        lhsVal += mapFigureToValue(lhs.targetPiece & NOTATION::PIECES::PIECES_MASK)
                - mapFigureToValue(lhs.sourcePiece & NOTATION::PIECES::PIECES_MASK);

        auto rhsVal = (rhs.flags & ExtendedMove::promotionMask) ? FIGURES_VALUE::QUEEN : 0;
        rhsVal += mapFigureToValue(rhs.targetPiece & NOTATION::PIECES::PIECES_MASK)
                  - mapFigureToValue(rhs.sourcePiece & NOTATION::PIECES::PIECES_MASK);

        return lhsVal > rhsVal;
    }
};

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
                            && (moveIt->flags & (ExtendedMove::beatingMask | ExtendedMove::promotionMask)) != 0
                            && ((moveIt-1)->flags & (ExtendedMove::beatingMask | ExtendedMove::promotionMask)) != 0)
                        {
                            auto firstNotBeating = std::find_if(ce_.precalculatedMoves.begin(),
                                                             ce_.precalculatedMoves.end(),
                                                             [](auto&& move){
                                return !(move.flags & (ExtendedMove::beatingMask | ExtendedMove::promotionMask));
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

        if (beatingAndPromotionsAnalyzed == false)
        {
            beatingAndPromotionsAnalyzed = true;
            auto firstNotBeatingIt = std::find_if(ce_.precalculatedMoves.begin() + index,
                ce_.precalculatedMoves.end(),
                [](auto&& move) {
                    return !(move.flags & (ExtendedMove::beatingMask | ExtendedMove::promotionMask));
            });
            nonPromotionNorBeatingIndex = firstNotBeatingIt - ce_.precalculatedMoves.begin();
            std::sort(ce_.precalculatedMoves.begin() + index, firstNotBeatingIt, MVVLVA_Comparator::compare);
        }

        if (index < nonPromotionNorBeatingIndex)
        {
            if (beatingAndPromotionsAnalyzed == false)
            {
                beatingAndPromotionsAnalyzed = true;
            }
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
    bool beatingAndPromotionsAnalyzed = false;
    bool movesSortedByHistory = false;
    int index;
    int nonPromotionNorBeatingIndex = 0;
};

class OnlyBeatingMoves
{
public:
    OnlyBeatingMoves(std::vector<ExtendedMove>&& moves)
            : moves_(std::move(moves))
            , index(0)
    {
        auto firstNotBeatingIt = std::find_if(moves_.begin() + index,
                                              moves_.end(),
                                              [](auto&& move) {
                                                  return !(move.flags & (ExtendedMove::beatingMask | ExtendedMove::promotionMask));
                                              });
        nonPromotionNorBeatingIndex = firstNotBeatingIt - moves_.begin();
        std::sort(moves_.begin() + index, firstNotBeatingIt, MVVLVA_Comparator::compare);
    }

    unsigned int size() const
    {
        return nonPromotionNorBeatingIndex;
    }
    ExtendedMove& get()
    {
        return moves_[index++];
    }
private:
    std::vector<ExtendedMove> moves_;
    int index;
    int nonPromotionNorBeatingIndex = 0;
};


int evaluatePosition(BoardEngine& be, players::common::move_generators::FullCachedEngine& moveGenerator)
{
    if(be.are3Repeatitions())
    {
        return 0;
    }

    return moveGenerator.getEvaluationValue(be) ;
}

int quiescenceSearch(BoardEngine& be,
                     players::common::move_generators::FullCachedEngine& cachedEngine,
                     unsigned char depth,
                     int alfa,
                     int beta)
{
    if (depth == 0) // terminate quiescence
    {
        return cachedEngine.getEvaluationValue(be);
    }

    auto moves = be.generateMoves();

    auto orderedMoves = OnlyBeatingMoves(std::move(moves));
    auto stablePosition = orderedMoves.size() == 0;
    if (stablePosition)
    {
        return cachedEngine.getEvaluationValue(be) ;
    }

    int stand_pat = evaluatePosition(be, cachedEngine);
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
    if (interrupt_flag)
    {
        return alfa;
    }
    if (depth == 0)
    {
        if(be.are3Repeatitions())
        {
            return 0;
        }
        // return evaluatePosition(be, cachedEngine);
        return quiescenceSearch(be, cachedEngine, 6/*Quinesence limit*/, alfa, beta);
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
            if (SaveMove and interrupt_flag == false) bestMove = move;
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
    if (SaveMove and interrupt_flag == false) bestMove = greatestMove;
    return alfa;
}

}  // namespace

namespace full_search
{

void interrupt()
{
    interrupt_flag = true;
}

Move evaluate(BoardEngine be,
              players::common::move_generators::FullCachedEngine& cachedEngine,
              unsigned char depth)
{
    interrupt_flag = false;
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
    interrupt_flag = false;
    int alpha = InitialAlpha;
    int beta = InitialBeta;
    memset(history, 0, sizeof(history[0][0][0]) * 2 * 64 * 64);

    for (auto depth = 2u; depth <= maxDepth; depth+=1)
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
