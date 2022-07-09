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



template <bool SaveMove>
int evaluateMax(BoardEngine& be,
                players::common::move_generators::FullCachedEngine& cachedEngine,
                unsigned char depth,
                int alfa,
                int beta);

ExtendedMove bestMove;
std::atomic_bool interrupt_flag = false;

unsigned history[2][64][64] = {};

class MVVLVA_Comparator // Most valuable victim less valuable aggressor
{
    static constexpr int weights[7][7]{
        // NOTHING  // PAWN,    ROCK,   KNIGHT,     BISHOP,     QUEEN,  KING
        {0,         0,          0,      0,          0,          0,      0}, //NOTHING
        {0,         106,        501,    306,        301,        901,    5000}, //PAWN
        {0,         104,        504,    304,        304,        904,    5000}, //ROCK
        {0,         105,        505,    305,        305,        905,    5000}, //KNIGHT
        {0,         105,        505,    305,        305,        905,    5000}, //BISHOP
        {0,         100,        500,    300,        300,        900,    5000}, //QUEEN
        {0,         107,        502,    307,        302,        902,    5000}, //KING
    };
public:
    static bool compare(const ExtendedMove& lhs, const ExtendedMove& rhs)
    {
        auto lhsVal = (lhs.flags & ExtendedMove::promotionMask) ? FIGURES_VALUE::QUEEN : 0;
        lhsVal += weights[lhs.sourcePiece >> 1][lhs.targetPiece >> 1];

        auto rhsVal = (rhs.flags & ExtendedMove::promotionMask) ? FIGURES_VALUE::QUEEN : 0;
        rhsVal += weights[rhs.sourcePiece >> 1][rhs.targetPiece >> 1];

        return lhsVal > rhsVal;
    }
};

void setHistoryMove(const NOTATION::COLOR::color player, ExtendedMove& move, unsigned char depth)
{
    if ((move.flags & ExtendedMove::beatingMask) == 0)
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
                    unsigned char depth,
                    std::vector<ExtendedMove>&& moves)
                    : historyMoves_(historyMovesPerSinglePlayer)
                    , ce_(ce)
                    , depth_(depth)
                    , index(0)
                    , moves_(std::move(moves))
    {
    }


    unsigned size()
    {
        return moves_.size();
    }

    ExtendedMove& get()
    {
        if (bestMoveAnalyzed == false)
        {
            bestMoveAnalyzed = true;
            for (auto candidateDepth = depth_; candidateDepth > 0; --candidateDepth)
            {
                if (ce_.previousBestMoves[candidateDepth].isSet)
                {
                    auto moveIt = std::find(moves_.begin(),
                                            moves_.end(),
                                            ce_.previousBestMoves[candidateDepth].move);
                    if (moveIt != moves_.end()) // It could be cache miss.
                                                                // Then no move like this exist in valid moves vec.
                    {
                        std::swap(*moveIt, moves_.front());
                        ++index;
                        if (moveIt != moves_.begin()
                            and not (
                                ce_.previousBestMoves[candidateDepth].move.flags &
                                (ExtendedMove::beatingMask | ExtendedMove::promotionMask)))
                        {
                            auto firstNotBeating = std::find_if(moves_.begin() + 1,
                                                                moves_.end(),
                                                             [](auto&& move){
                                return !(move.flags & (ExtendedMove::beatingMask | ExtendedMove::promotionMask));
                            });
                            if (firstNotBeating != moves_.end())
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
            auto firstNotBeatingIt = std::find_if(moves_.begin() + index,
                                                  moves_.end(),
                [](auto&& move) {
                    return !(move.flags & (ExtendedMove::beatingMask | ExtendedMove::promotionMask));
            });
            nonPromotionNorBeatingIndex = firstNotBeatingIt - moves_.begin();
            std::sort(moves_.begin() + index, firstNotBeatingIt, MVVLVA_Comparator::compare);
        }

        if (index < nonPromotionNorBeatingIndex)
        {
            return moves_[index++];
        }

        if (movesSortedByHistory == false)
        {
            movesSortedByHistory = true;
            std::sort(moves_.begin() + index,
                      moves_.end(),
                      [&](auto& lhs, auto& rhs) {
                return historyMoves_[lhs.source][lhs.destination] > historyMoves_[rhs.source][rhs.destination];
            });
        }
        return moves_[index++];
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
    std::vector<ExtendedMove> moves_;
};

class OnlyBeatingMoves
{
public:
    OnlyBeatingMoves(std::vector<ExtendedMove>&& moves)
            : moves_(std::move(moves))
            , index(0)
    {
        auto firstNotBeatingIt = std::find_if(moves_.begin(),
                                              moves_.end(),
                                              [](auto&& move) {
                                                  return !(move.flags & (ExtendedMove::beatingMask | ExtendedMove::promotionMask));
                                              });
        nonPromotionNorBeatingIndex = firstNotBeatingIt - moves_.begin();
        std::sort(moves_.begin(), firstNotBeatingIt, MVVLVA_Comparator::compare);
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
        if (cache.previousEvaluations[i].visitedBefore)
        {
            if (cache.previousEvaluations[i].lowerValue >= beta)
                return beta;
            if (cache.previousEvaluations[i].higherValue <= alfa)
                return alfa;
            alfa = std::max(alfa, cache.previousEvaluations[i].lowerValue);
            beta = std::min(beta, cache.previousEvaluations[i].higherValue);
            break;
        }
    }

    auto memorial = be.getMemorial();
    ExtendedMove greatestMove;
    auto nextAlfa = -1000000;
    auto pvFound = false;

    auto orderedMoves = PreorderedMoves(
            history[((be.board.playerOnMove == NOTATION::COLOR::color::white) ? 0 : 1)],
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
            // setKiller(move, depth);
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
    inline ExtendedMove evaluateIterative(BoardEngine be,
                       players::common::move_generators::FullCachedEngine& cachedEngine,
                       unsigned char maxDepth)
{
    interrupt_flag = false;
    int alpha = InitialAlpha;
    int beta = InitialBeta;

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
