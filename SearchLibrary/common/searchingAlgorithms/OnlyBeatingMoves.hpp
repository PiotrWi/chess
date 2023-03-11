#pragma once

#include <core/Board.hpp>
#include <utility>

/*
 * Likely, will not be used anymore
 */

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