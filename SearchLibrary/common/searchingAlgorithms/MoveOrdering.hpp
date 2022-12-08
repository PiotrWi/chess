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

#include "PositionAnalyzers/sse.hpp"

static unsigned history[2][64][64] = {};

inline void setHistoryMove(const NOTATION::COLOR::color player, const ExtendedMove& move, unsigned char depth);
inline void clearHistoryMove();

template <typename TIterator>
TIterator getMinimumRecaptureMove(TIterator it, TIterator end, unsigned char destination)
{
    TIterator currentLow = end;
    for (; it != end; ++it)
    {
        if (it->destination == destination && (currentLow == end or PiecesValuesTable[currentLow->sourcePiece] > PiecesValuesTable[it->sourcePiece]))
        {
            if (PiecesValuesTable[it->sourcePiece] == PawnValue)  // :|
            {
                return it;
            }
            currentLow = it;
        }
    }
    return currentLow;
}
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
    OnlyBeatingMoves(std::span<ExtendedMove>&& moves, const ExtendedMove& lastMove);
    unsigned int size() const;
    const ExtendedMove& get();
private:
    const ExtendedMove& getRecaptureMove();
    const ExtendedMove& prepareMove();
    const ExtendedMove& getMove();

    std::span<ExtendedMove> moves_;
    const ExtendedMove& (OnlyBeatingMoves::*currentGetterFunction)() = &OnlyBeatingMoves::getRecaptureMove;

    int index;
    const ExtendedMove& lastMove_;
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


inline OnlyBeatingMoves::OnlyBeatingMoves(std::span<ExtendedMove>&& moves, const ExtendedMove& lastMove)
        : moves_(std::move(moves))
        , index(0)
        , lastMove_(lastMove)
{
}

inline unsigned int OnlyBeatingMoves::size() const
{
    return moves_.size();
}

inline const ExtendedMove& OnlyBeatingMoves::getRecaptureMove()
{
    if (lastMove_.flags & (ExtendedMove::beatingMask | ExtendedMove::promotionMask))
    {
        auto recapture = getMinimumRecaptureMove(moves_.begin(), moves_.end(), lastMove_.destination);
        if (recapture != moves_.end())
        {
            std::swap(*recapture, *moves_.begin());
            currentGetterFunction = &OnlyBeatingMoves::prepareMove;
            ++index;
            return *moves_.begin();
        }
    }
    currentGetterFunction = &OnlyBeatingMoves::prepareMove;
    return (this->*currentGetterFunction)();
}

inline const ExtendedMove& OnlyBeatingMoves::prepareMove()
{
    std::sort(moves_.begin() + index, moves_.end(), MVVLVA_Comparator::compare);
    currentGetterFunction = &OnlyBeatingMoves::getMove;
    return (this->*currentGetterFunction)();
}

inline const ExtendedMove& OnlyBeatingMoves::getMove()
{
    return moves_[index++];
}

inline const ExtendedMove& OnlyBeatingMoves::get()
{
    return (this->*currentGetterFunction)();
}

class OnlyBeatingMovesSeeVersion
{
public:
    OnlyBeatingMovesSeeVersion(std::span<ExtendedMove>&& moves, const Board& b, const ExtendedMove& lastMove, int trigger);
    unsigned int size() const;
    std::optional<ExtendedMove> get();
private:
    std::optional<ExtendedMove> getRecaptureMove();
    std::optional<ExtendedMove> prepareMove();
    std::optional<ExtendedMove> getMove();

    std::optional<ExtendedMove> (OnlyBeatingMovesSeeVersion::*currentGetterFunction)() = &OnlyBeatingMovesSeeVersion::getRecaptureMove;
    std::span<ExtendedMove> moves_;
    boost::container::small_vector<std::pair<int, std::span<ExtendedMove>::iterator>, 30> sortedBySee;
    int index;
    const Board& board_;
    const ExtendedMove& lastMove_;
    const int margin_;
};

inline bool operator<(const std::pair<int, std::span<ExtendedMove>::iterator>& lhs, const std::pair<int, std::span<ExtendedMove>::iterator>& rhs)
{
    return lhs.first < rhs.first;
}

inline OnlyBeatingMovesSeeVersion::OnlyBeatingMovesSeeVersion(std::span<ExtendedMove>&& moves, const Board& b, const ExtendedMove& lastMove, int margin)
        : moves_(std::move(moves))
        , index(0)
        , board_(b)
        , lastMove_(lastMove)
        , margin_(margin)
{
    if (margin > 1800)
    {
        return;
    }
}

inline unsigned int OnlyBeatingMovesSeeVersion::size() const
{
    return sortedBySee.size();
}

inline std::optional<ExtendedMove> OnlyBeatingMovesSeeVersion::get()
{
    return (this->*currentGetterFunction)();
}

inline std::optional<ExtendedMove> OnlyBeatingMovesSeeVersion::getRecaptureMove()
{
    if (lastMove_.flags & (ExtendedMove::beatingMask | ExtendedMove::promotionMask))
    {
        auto recapture = getMinimumRecaptureMove(moves_.begin(), moves_.end(), lastMove_.destination);
        if (recapture != moves_.end()
            && see(*recapture, board_, board_.playerOnMove) >= (margin_ - 250))
        {
            std::swap(*recapture, *moves_.begin());
            currentGetterFunction = &OnlyBeatingMovesSeeVersion::prepareMove;
            ++index;
            return *moves_.begin();
        }
    }
    currentGetterFunction = &OnlyBeatingMovesSeeVersion::prepareMove;
    return (this->*currentGetterFunction)();
}

inline std::optional<ExtendedMove> OnlyBeatingMovesSeeVersion::prepareMove()
{
    for (auto it = moves_.begin() + index; it != moves_.end(); ++it)
    {
        auto seeVal = see(*it, board_, board_.playerOnMove);
        if (seeVal >= (margin_ - 250))
        {
            sortedBySee.emplace_back(seeVal, it);
        }
    }
    index = 0;
    std::sort(sortedBySee.rbegin(), sortedBySee.rend());
    currentGetterFunction = &OnlyBeatingMovesSeeVersion::getMove;
    return (this->*currentGetterFunction)();
}

inline std::optional<ExtendedMove> OnlyBeatingMovesSeeVersion::getMove()
{
    if (index < (int)sortedBySee.size())
    {
        return *sortedBySee[index++].second;
    }
    return {};
}
