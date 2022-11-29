#pragma once

#include <BoardEngine.hpp>

#include <atomic>
#include <algorithm>
#include <cstring>
#include <vector>

#include <optional>

#include <common/CachedEngines/FullCachedEngine.hpp>

#include <publicIf/NotationConversions.hpp>
#include <common/Constants.hpp>
#include <MoveGeneratorV2/MoveGeneratorV2.hpp>

#include <detail/sse.hpp>

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
                    MoveGenerator::MoveGeneratorV2& mg);

    std::optional<ExtendedMove> get();
private:
    std::optional<ExtendedMove> getBestMove();
    std::optional<ExtendedMove> getBeatingPrepare();
    std::optional<ExtendedMove> getBeating();
    std::optional<ExtendedMove> getHistoryPrepare();
    std::optional<ExtendedMove> getHistory();

    std::optional<ExtendedMove>  (PreorderedMoves::*currentGetterFunction)() = &PreorderedMoves::getBestMove;

    decltype(history[0])& historyMoves_;
    players::common::move_generators::CacheFullEntity* ce_;
    unsigned char depth_;
    MoveGenerator::MoveGeneratorV2& mg_;
    std::span<ExtendedMove> moves_;
    std::span<ExtendedMove>::iterator it_;
    std::optional<ExtendedMove> bestMove_ = std::nullopt;
};

class OnlyBeatingMoves
{
public:
    OnlyBeatingMoves(std::span<ExtendedMove>&& moves);
    unsigned int size() const;
    ExtendedMove& get();
private:
    std::span<ExtendedMove> moves_;
    int index;
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
                    MoveGenerator::MoveGeneratorV2& mg)
                    : historyMoves_(history[((playerOnMove == NOTATION::COLOR::color::white) ? 0 : 1)])
                    , ce_(ce)
                    , depth_(depth)
                    , mg_(mg){}

inline std::optional<ExtendedMove> PreorderedMoves::get()
{
    return (this->*currentGetterFunction)();
}

inline std::optional<ExtendedMove> PreorderedMoves::getBestMove()
{
    for (auto candidateDepth = depth_; candidateDepth > 0; --candidateDepth)
    {
        if (ce_->previousBestMoves[candidateDepth].isSet)
        {
            {
                bestMove_ = ce_->previousBestMoves[candidateDepth].move;
                currentGetterFunction = &PreorderedMoves::getBeatingPrepare;
                return bestMove_;
            }
        }
    }
    currentGetterFunction = &PreorderedMoves::getBeatingPrepare;
    return (this->*currentGetterFunction)();
}

inline std::optional<ExtendedMove> PreorderedMoves::getBeatingPrepare()
{
    moves_ = mg_.generateBeatingMoves();
    it_ = moves_.begin();
    if (bestMove_ and bestMove_->flags & (ExtendedMove::beatingMask | ExtendedMove::promotionMask))
    {
        auto bmIt = std::find(moves_.begin(), moves_.end(), *bestMove_);
        if (bmIt != std::end(moves_))
        {
            std::swap(*bmIt, moves_.front());
            ++it_;
        }
    } 
    std::sort(it_, moves_.end(), MVVLVA_Comparator::compare);

    currentGetterFunction = &PreorderedMoves::getBeating;
    return (this->*currentGetterFunction)();
}

inline std::optional<ExtendedMove> PreorderedMoves::getBeating()
{
    if (it_ != moves_.end())
    {
        auto& elem = *it_;
        ++it_;
        return elem;
    }
    currentGetterFunction = &PreorderedMoves::getHistoryPrepare;
    return (this->*currentGetterFunction)();
}

inline std::optional<ExtendedMove> PreorderedMoves::getHistoryPrepare()
{
    moves_ = mg_.generateNonBeatingMoves();
    it_ = moves_.begin();
    if (bestMove_)
    {
        auto bmIt = std::find(moves_.begin(), moves_.end(), *bestMove_);
        if (bmIt != moves_.end())
        {
            std::swap(*bmIt, moves_.front());
            ++it_;
        }
    } 
    std::sort(it_, moves_.end(), [&](auto& lhs, auto& rhs) {
        return historyMoves_[lhs.source][lhs.destination] > historyMoves_[rhs.source][rhs.destination];
    });
    currentGetterFunction = &PreorderedMoves::getHistory;
    return (this->*currentGetterFunction)();
}

inline std::optional<ExtendedMove> PreorderedMoves::getHistory()
{
    if (it_ != moves_.end())
    {
        auto& elem = *it_;
        ++it_;
        return elem;
    }
    return {};
}


inline OnlyBeatingMoves::OnlyBeatingMoves(std::span<ExtendedMove>&& moves)
        : moves_(std::move(moves))
        , index(0)
{
    std::sort(moves_.begin(), moves_.end(), MVVLVA_Comparator::compare);
}

inline unsigned int OnlyBeatingMoves::size() const
{
    return moves_.size();
}

inline ExtendedMove& OnlyBeatingMoves::get()
{
    return moves_[index++];
}

/*Suprizingly, gives no adventage at all.*/
/* Maybe recapturing? */
class OnlyBeatingMovesSeeVersion
{
public:
    OnlyBeatingMovesSeeVersion(std::span<ExtendedMove>&& moves, const Board& b);
    unsigned int size() const;
    ExtendedMove& get();
private:
    std::span<ExtendedMove> moves_;
    boost::container::small_vector<std::pair<int, std::span<ExtendedMove>::iterator>, 30> sortedBySee;
    int index;
};

inline bool operator<(const std::pair<int, std::span<ExtendedMove>::iterator>& lhs, const std::pair<int, std::span<ExtendedMove>::iterator>& rhs)
{
    return lhs.first < rhs.first;
}

inline OnlyBeatingMovesSeeVersion::OnlyBeatingMovesSeeVersion(std::span<ExtendedMove>&& moves, const Board& b)
        : moves_(std::move(moves))
        , index(0)
{
    // std::sort(moves_.begin(), moves_.end(), MVVLVA_Comparator::compare);
    for (auto it = moves_.begin(); it != moves_.end(); ++it)
    {
        sortedBySee.emplace_back(see(*it, b, b.playerOnMove), it);
    }
    std::sort(sortedBySee.rbegin(), sortedBySee.rend());
}

inline unsigned int OnlyBeatingMovesSeeVersion::size() const
{
    auto it = std::find_if(sortedBySee.begin(), sortedBySee.end(), [&](auto&& elem) { return elem.first <= 0; } );
    return it - sortedBySee.begin();
    // return moves_.size();
}

inline ExtendedMove& OnlyBeatingMovesSeeVersion::get()
{
    return *sortedBySee[index++].second;
}