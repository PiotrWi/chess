#pragma once

#include <BoardEngine.hpp>

#include <atomic>
#include <algorithm>
#include <cstring>
#include <vector>
#include <common/CachedEngines/FullCachedEngine.hpp>

#include <publicIf/NotationConversions.hpp>
#include <common/Constants.hpp>

static unsigned history[2][64][64] = {};

inline void setHistoryMove(const NOTATION::COLOR::color player, const ExtendedMove& move, unsigned char depth);
inline void clearHistoryMove();

class MVVLVA_Comparator // Most valuable victim less valuable aggressor
{
    static constexpr int weights[7][7]{
        // NOTHING  // PAWN,    ROCK,   KNIGHT,     BISHOP,     QUEEN,  KING
        {0,         0,          0,      0,          0,          0,      0}, //NOTHING
        {0,         106,        506,    306,        306,        906,    5000}, //PAWN
        {0,         101,        501,    301,        301,        901,    5000}, //ROCK
        {0,         105,        505,    305,        305,        905,    5000}, //KNIGHT
        {0,         104,        504,    304,        304,        904,    5000}, //BISHOP
        {0,         100,        500,    300,        300,        900,    5000}, //QUEEN
        {0,         107,        507,    307,        307,        907,    5000}, //KING
    };
public:
    static bool compare(const ExtendedMove& lhs, const ExtendedMove& rhs);
};

class PreorderedMoves
{
public:
    PreorderedMoves(NOTATION::COLOR::color playerOnMove,
                    players::common::move_generators::CacheFullEntity* ce,
                    unsigned char depth,
                    std::vector<ExtendedMove>&& moves);

    unsigned size() const;
    ExtendedMove& get();
private:
    ExtendedMove& getBestMove();
    ExtendedMove& getBeatingPrepare();
    ExtendedMove& getBeating();
    ExtendedMove& getHistoryPrepare();
    ExtendedMove& getHistory();

    ExtendedMove& (PreorderedMoves::*currentGetterFunction)() = &PreorderedMoves::getBestMove;

    decltype(history[0])& historyMoves_;
    players::common::move_generators::CacheFullEntity* ce_;
    unsigned char depth_;
    int index;
    int nonPromotionNorBeatingIndex = 0;
    std::vector<ExtendedMove> moves_;
    std::vector<ExtendedMove>::iterator end;
};

class OnlyBeatingMoves
{
public:
    OnlyBeatingMoves(std::vector<ExtendedMove>&& moves);
    unsigned int size() const;
    ExtendedMove& get();
private:
    std::vector<ExtendedMove> moves_;
    int index;
    int nonPromotionNorBeatingIndex = 0;
};

inline void setHistoryMove(const NOTATION::COLOR::color player, const ExtendedMove& move, unsigned char depth)
{
    if ((move.flags & ExtendedMove::beatingMask) == 0)
    {
        history[static_cast<unsigned char >(player)][move.source][move.destination] +=
                depth * depth;
    }
}

inline void clearHistoryMove()
{
    memset(history, 0, sizeof(history[0][0][0]) * 2 * 64 * 64);
}

inline bool MVVLVA_Comparator::compare(const ExtendedMove& lhs, const ExtendedMove& rhs)
{
    auto lhsVal = (lhs.flags & ExtendedMove::promotionMask) ? FIGURES_VALUE::QUEEN : 0;
    lhsVal += weights[lhs.sourcePiece >> 1][lhs.targetPiece >> 1];

    auto rhsVal = (rhs.flags & ExtendedMove::promotionMask) ? FIGURES_VALUE::QUEEN : 0;
    rhsVal += weights[rhs.sourcePiece >> 1][rhs.targetPiece >> 1];

    return lhsVal > rhsVal;
}

inline PreorderedMoves::PreorderedMoves(NOTATION::COLOR::color playerOnMove,
                    players::common::move_generators::CacheFullEntity* ce,
                    unsigned char depth,
                    std::vector<ExtendedMove>&& moves)
                    : historyMoves_(history[((playerOnMove == NOTATION::COLOR::color::white) ? 0 : 1)])
                    , ce_(ce)
                    , depth_(depth)
                    , index(0)
                    , moves_(std::move(moves))
                    , end(std::end(moves_)) {}

inline unsigned PreorderedMoves::size() const
{
    return moves_.size();
}

inline ExtendedMove& PreorderedMoves::get()
{
    return (this->*currentGetterFunction)();
}

inline ExtendedMove& PreorderedMoves::getBestMove()
{
    for (auto candidateDepth = depth_; candidateDepth > 0; --candidateDepth)
    {
        if (ce_->previousBestMoves[candidateDepth].isSet)
        {
            auto moveIt = std::find(moves_.begin(),
                                    end,
                                    ce_->previousBestMoves[candidateDepth].move);
            if (moveIt != end) // It could be cache miss.
                                        // Then no move like this may exist in valid moves vec.
            {
                auto isBeating = moveIt->flags & (ExtendedMove::beatingMask | ExtendedMove::promotionMask);
                if (isBeating)
                {
                    *moveIt = moves_.front();
                    ++index;
                    currentGetterFunction = &PreorderedMoves::getBeatingPrepare;
                    return ce_->previousBestMoves[candidateDepth].move;
                }
                *moveIt = moves_.back();
                --end;
                currentGetterFunction = &PreorderedMoves::getBeatingPrepare;
                return ce_->previousBestMoves[candidateDepth].move;
            }
        }
    }
    currentGetterFunction = &PreorderedMoves::getBeatingPrepare;
    return (this->*currentGetterFunction)();
}

inline ExtendedMove& PreorderedMoves::getBeatingPrepare()
{
    auto firstNotBeatingIt = std::find_if(moves_.begin() + index,
                                          end,
        [](auto&& move) {
            return !(move.flags & (ExtendedMove::beatingMask | ExtendedMove::promotionMask));
    });
    nonPromotionNorBeatingIndex = firstNotBeatingIt - moves_.begin();
    std::sort(moves_.begin() + index, firstNotBeatingIt, MVVLVA_Comparator::compare);

    currentGetterFunction = &PreorderedMoves::getBeating;
    return (this->*currentGetterFunction)();
}

inline ExtendedMove& PreorderedMoves::getBeating()
{
    if (index < nonPromotionNorBeatingIndex)
    {
        return moves_[index++];
    }
    currentGetterFunction = &PreorderedMoves::getHistoryPrepare;
    return (this->*currentGetterFunction)();
}

inline ExtendedMove& PreorderedMoves::getHistoryPrepare()
{
    std::stable_sort(moves_.begin() + index,
              end,
              [&](auto& lhs, auto& rhs) {
        return historyMoves_[lhs.source][lhs.destination] > historyMoves_[rhs.source][rhs.destination];
    });
    currentGetterFunction = &PreorderedMoves::getHistory;
    return (this->*currentGetterFunction)();
}

inline ExtendedMove& PreorderedMoves::getHistory()
{

    return moves_[index++];
}

inline OnlyBeatingMoves::OnlyBeatingMoves(std::vector<ExtendedMove>&& moves)
        : moves_(std::move(moves))
        , index(0)
{
    auto firstNotBeatingIt = std::find_if(moves_.begin(),
                                          moves_.end(),
                                          [](auto&& move) {
                                              return !(move.flags & (ExtendedMove::beatingMask | ExtendedMove::promotionMask));
                                          });
    nonPromotionNorBeatingIndex = firstNotBeatingIt - moves_.begin();
    std::stable_sort(moves_.begin(), firstNotBeatingIt, MVVLVA_Comparator::compare);
}

inline unsigned int OnlyBeatingMoves::size() const
{
    return nonPromotionNorBeatingIndex;
}

inline ExtendedMove& OnlyBeatingMoves::get()
{
    return moves_[index++];
}
