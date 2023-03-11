#pragma once

#include "MVVLVA.hpp"

#include <algorithm>
#include <cstring>
#include <vector>
#include <optional>

#include <BoardEngine.hpp>
#include <common/CachedEngines/FullCachedEngine.hpp>
#include <core/NotationConversions.hpp>
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


class PreorderedMoves
{
public:
    PreorderedMoves(NOTATION::COLOR::color playerOnMove,
                    players::common::move_generators::CacheFullEntity* ce,
                    unsigned char depth,
                    MoveGenerator::MoveGeneratorV2& mg,
                    const ExtendedMove& lastMove,
                    const Board& board);

    std::optional<ExtendedMove> get();
private:
    std::optional<ExtendedMove> getBestMove();
    std::optional<ExtendedMove> getBeatingPrepare();
    std::optional<ExtendedMove> getRecaptureMove();
    std::optional<ExtendedMove> sortBeatingMoves();
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
    const ExtendedMove& lastMove_;
    const Board& board_;
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

struct MoveWithSeeAndMvvlva
{
    int seeVal = 0;
    int mvvlvaVal = 0;
    ExtendedMove move;
};

inline bool operator<(const MoveWithSeeAndMvvlva& lhs, const MoveWithSeeAndMvvlva& rhs)
{
    if (lhs.seeVal == rhs.seeVal)
        return lhs.mvvlvaVal < rhs.mvvlvaVal;
    return lhs.seeVal < rhs.seeVal;
}

inline PreorderedMoves::PreorderedMoves(NOTATION::COLOR::color playerOnMove,
                    players::common::move_generators::CacheFullEntity* ce,
                    unsigned char depth,
                    MoveGenerator::MoveGeneratorV2& mg,
                    const ExtendedMove& lastMove,
                    const Board& board)
                    : historyMoves_(history[((playerOnMove == NOTATION::COLOR::color::white) ? 0 : 1)])
                    , ce_(ce)
                    , depth_(depth)
                    , mg_(mg)
                    , lastMove_(lastMove)
                    , board_(board)
                    {}

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

    currentGetterFunction = &PreorderedMoves::sortBeatingMoves;
    // currentGetterFunction = &PreorderedMoves::getRecaptureMove;
    return (this->*currentGetterFunction)();
}

inline std::optional<ExtendedMove> PreorderedMoves::getRecaptureMove()
{
    if (lastMove_.flags & (ExtendedMove::beatingMask | ExtendedMove::promotionMask))
    {
        auto recapture = getMinimumRecaptureMove(it_, moves_.end(), lastMove_.destination);
        if (recapture != moves_.end())
        {
            std::swap(*recapture, *it_);
            ++it_;
            currentGetterFunction = &PreorderedMoves::sortBeatingMoves;
            return *moves_.begin();
        }
    }
    currentGetterFunction = &PreorderedMoves::sortBeatingMoves;
    return (this->*currentGetterFunction)();
}

inline bool operator<(const std::pair<int, std::span<ExtendedMove>::iterator>& lhs, const std::pair<int, std::span<ExtendedMove>::iterator>& rhs)
{
    return lhs.first < rhs.first;
}


inline std::optional<ExtendedMove> PreorderedMoves::sortBeatingMoves()
{
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

class OnlyBeatingMovesSeeVersion
{
public:
    OnlyBeatingMovesSeeVersion(std::span<ExtendedMove>&& moves, const Board& b, const ExtendedMove& lastMove, int trigger);
    std::optional<ExtendedMove> get();
    void setMargin(int trigger) { margin_ = trigger; };
private:
    std::optional<ExtendedMove> getRecaptureMove();
    std::optional<ExtendedMove> prepareMove();
    std::optional<ExtendedMove> getMove();

    std::optional<ExtendedMove> (OnlyBeatingMovesSeeVersion::*currentGetterFunction)() = &OnlyBeatingMovesSeeVersion::getRecaptureMove;
    std::span<ExtendedMove> moves_;
    int index;
    const Board& board_;
    const ExtendedMove& lastMove_;
    int margin_;
};

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
    std::sort(moves_.begin() + index, moves_.end(), MVVLVA_Comparator::compare);
    currentGetterFunction = &OnlyBeatingMovesSeeVersion::getMove;
    return (this->*currentGetterFunction)();
}

inline std::optional<ExtendedMove> OnlyBeatingMovesSeeVersion::getMove()
{
    for (; index < (int)moves_.size(); ++index)
    {
        auto seeVal = see(moves_[index], board_, board_.playerOnMove);
        if (seeVal >= (margin_ - 250))
            return moves_[index++];
    }
    return {};
}
