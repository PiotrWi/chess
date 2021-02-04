#pragma once
#include <FieldLookup/FieldLookup.hpp>
#include <publicIf/Board.hpp>

namespace
{
template<NOTATION::COLOR::color c, unsigned char i, const unsigned char TSize, const unsigned char *begin>
uint64_t evaluateInSingleDiagonal(uint64_t wasChecked, const Board& board)
{
    auto piecePos = 0;
    constexpr auto TOppositeColor = c + 1;

    for (const unsigned char *pos = begin;
         pos < begin + TSize;
         ++pos)
    {
        if ((board.getField(*pos) & NOTATION::COLOR::COLOR_MASK) == TOppositeColor)
        {
            break;
        }
        if ((board.getField(*pos) & NOTATION::COLOR::COLOR_MASK) == c)
        {
            piecePos = *pos;
            for (++pos;
                 pos < begin + TSize;
                 ++pos)
            {
                if ((board.getField(*pos) & NOTATION::COLOR::COLOR_MASK) == TOppositeColor)
                {
                    if ((board.getField(*pos) & NOTATION::PIECES::BISHOP) or
                        (board.getField(*pos) & NOTATION::PIECES::QUEEN))
                    {
                        return wasChecked | (1lu << piecePos);
                    }
                }
                if ((board.getField(*pos) & NOTATION::COLOR::COLOR_MASK) == c)
                {
                    return wasChecked;
                }
            }
        }
    }
    return wasChecked;
}

template<NOTATION::COLOR::color c, unsigned char i, const unsigned char TSize, const unsigned char *begin>
uint64_t evaluateInSingleLine(uint64_t wasChecked, const Board& board)
{
    auto piecePos = 0;
    constexpr auto TOppositeColor = c + 1;
    for (const unsigned char *pos = begin;
         pos < begin + TSize;
         ++pos)
    {
        if ((board.getField(*pos) & NOTATION::COLOR::COLOR_MASK) == TOppositeColor)
        {
            break;
        }
        if ((board.getField(*pos) & NOTATION::COLOR::COLOR_MASK) == c)
        {
            piecePos = *pos;

            for (++pos;
                 pos < begin + TSize;
                 ++pos)
            {
                if ((board.getField(*pos) & NOTATION::COLOR::COLOR_MASK) == TOppositeColor)
                {
                    if ((board.getField(*pos) & NOTATION::PIECES::ROCK) or (board.getField(*pos) & NOTATION::PIECES::QUEEN))
                    {
                        return wasChecked | (1lu << piecePos);
                    }
                }
                if ((board.getField(*pos) & NOTATION::COLOR::COLOR_MASK) == c)
                {
                    return wasChecked;
                }
            }
        }
    }
    return wasChecked;
}

template <NOTATION::COLOR::color c, unsigned char TKingPos>
uint64_t findPinned(const Board& board)
{
    uint64_t pinned = 0;

    pinned = evaluateInSingleLine<c, TKingPos,
            FieldLookup<c, TKingPos>::TopPossibleMovesSize,
            FieldLookup<c, TKingPos>::TopPossibleMoves>(pinned, board);
    pinned = evaluateInSingleLine<c, TKingPos,
            FieldLookup<c, TKingPos>::BottomPossibleMovesSize,
            FieldLookup<c, TKingPos>::BottomPossibleMoves>(pinned, board);
    pinned = evaluateInSingleLine<c, TKingPos,
            FieldLookup<c, TKingPos>::LeftPossibleMovesSize,
            FieldLookup<c, TKingPos>::LeftPossibleMoves>(pinned, board);
    pinned = evaluateInSingleLine<c, TKingPos,
            FieldLookup<c, TKingPos>::RightPossibleMovesSize,
            FieldLookup<c, TKingPos>::RightPossibleMoves>(pinned, board);
    pinned = evaluateInSingleDiagonal<c, TKingPos,
            FieldLookup<c, TKingPos>::TopLeftPossibleMovesSize,
            FieldLookup<c, TKingPos>::TopLeftPossibleMoves>(pinned, board);
    pinned = evaluateInSingleDiagonal<c, TKingPos,
            FieldLookup<c, TKingPos>::TopRightPossibleMovesSize,
            FieldLookup<c, TKingPos>::TopRightPossibleMoves>(pinned, board);
    pinned = evaluateInSingleDiagonal<c, TKingPos,
            FieldLookup<c, TKingPos>::BottomLeftPossibleMovesSize,
            FieldLookup<c, TKingPos>::BottomLeftPossibleMoves>(pinned, board);
    pinned = evaluateInSingleDiagonal<c, TKingPos,
            FieldLookup<c, TKingPos>::BottomRightPossibleMovesSize,
            FieldLookup<c, TKingPos>::BottomRightPossibleMoves>(pinned, board);

    return pinned;
}

static uint64_t (*whiteLookup[])(const Board& board)  = {
        findPinned<NOTATION::COLOR::color::white, 0>,
        findPinned<NOTATION::COLOR::color::white, 1>,
        findPinned<NOTATION::COLOR::color::white, 2>,
        findPinned<NOTATION::COLOR::color::white, 3>,
        findPinned<NOTATION::COLOR::color::white, 4>,
        findPinned<NOTATION::COLOR::color::white, 5>,
        findPinned<NOTATION::COLOR::color::white, 6>,
        findPinned<NOTATION::COLOR::color::white, 7>,
        findPinned<NOTATION::COLOR::color::white, 8>,
        findPinned<NOTATION::COLOR::color::white, 9>,
        findPinned<NOTATION::COLOR::color::white, 10>,
        findPinned<NOTATION::COLOR::color::white, 11>,
        findPinned<NOTATION::COLOR::color::white, 12>,
        findPinned<NOTATION::COLOR::color::white, 13>,
        findPinned<NOTATION::COLOR::color::white, 14>,
        findPinned<NOTATION::COLOR::color::white, 15>,
        findPinned<NOTATION::COLOR::color::white, 16>,
        findPinned<NOTATION::COLOR::color::white, 17>,
        findPinned<NOTATION::COLOR::color::white, 18>,
        findPinned<NOTATION::COLOR::color::white, 19>,
        findPinned<NOTATION::COLOR::color::white, 20>,
        findPinned<NOTATION::COLOR::color::white, 21>,
        findPinned<NOTATION::COLOR::color::white, 22>,
        findPinned<NOTATION::COLOR::color::white, 23>,
        findPinned<NOTATION::COLOR::color::white, 24>,
        findPinned<NOTATION::COLOR::color::white, 25>,
        findPinned<NOTATION::COLOR::color::white, 26>,
        findPinned<NOTATION::COLOR::color::white, 27>,
        findPinned<NOTATION::COLOR::color::white, 28>,
        findPinned<NOTATION::COLOR::color::white, 29>,
        findPinned<NOTATION::COLOR::color::white, 30>,
        findPinned<NOTATION::COLOR::color::white, 31>,
        findPinned<NOTATION::COLOR::color::white, 32>,
        findPinned<NOTATION::COLOR::color::white, 33>,
        findPinned<NOTATION::COLOR::color::white, 34>,
        findPinned<NOTATION::COLOR::color::white, 35>,
        findPinned<NOTATION::COLOR::color::white, 36>,
        findPinned<NOTATION::COLOR::color::white, 37>,
        findPinned<NOTATION::COLOR::color::white, 38>,
        findPinned<NOTATION::COLOR::color::white, 39>,
        findPinned<NOTATION::COLOR::color::white, 40>,
        findPinned<NOTATION::COLOR::color::white, 41>,
        findPinned<NOTATION::COLOR::color::white, 42>,
        findPinned<NOTATION::COLOR::color::white, 43>,
        findPinned<NOTATION::COLOR::color::white, 44>,
        findPinned<NOTATION::COLOR::color::white, 45>,
        findPinned<NOTATION::COLOR::color::white, 46>,
        findPinned<NOTATION::COLOR::color::white, 47>,
        findPinned<NOTATION::COLOR::color::white, 48>,
        findPinned<NOTATION::COLOR::color::white, 49>,
        findPinned<NOTATION::COLOR::color::white, 50>,
        findPinned<NOTATION::COLOR::color::white, 51>,
        findPinned<NOTATION::COLOR::color::white, 52>,
        findPinned<NOTATION::COLOR::color::white, 53>,
        findPinned<NOTATION::COLOR::color::white, 54>,
        findPinned<NOTATION::COLOR::color::white, 55>,
        findPinned<NOTATION::COLOR::color::white, 56>,
        findPinned<NOTATION::COLOR::color::white, 57>,
        findPinned<NOTATION::COLOR::color::white, 58>,
        findPinned<NOTATION::COLOR::color::white, 59>,
        findPinned<NOTATION::COLOR::color::white, 60>,
        findPinned<NOTATION::COLOR::color::white, 61>,
        findPinned<NOTATION::COLOR::color::white, 62>,
        findPinned<NOTATION::COLOR::color::white, 63>,
};

static uint64_t(*blackLookup[])(const Board& board)  = {
        findPinned<NOTATION::COLOR::color::black, 0>,
        findPinned<NOTATION::COLOR::color::black, 1>,
        findPinned<NOTATION::COLOR::color::black, 2>,
        findPinned<NOTATION::COLOR::color::black, 3>,
        findPinned<NOTATION::COLOR::color::black, 4>,
        findPinned<NOTATION::COLOR::color::black, 5>,
        findPinned<NOTATION::COLOR::color::black, 6>,
        findPinned<NOTATION::COLOR::color::black, 7>,
        findPinned<NOTATION::COLOR::color::black, 8>,
        findPinned<NOTATION::COLOR::color::black, 9>,
        findPinned<NOTATION::COLOR::color::black, 10>,
        findPinned<NOTATION::COLOR::color::black, 11>,
        findPinned<NOTATION::COLOR::color::black, 12>,
        findPinned<NOTATION::COLOR::color::black, 13>,
        findPinned<NOTATION::COLOR::color::black, 14>,
        findPinned<NOTATION::COLOR::color::black, 15>,
        findPinned<NOTATION::COLOR::color::black, 16>,
        findPinned<NOTATION::COLOR::color::black, 17>,
        findPinned<NOTATION::COLOR::color::black, 18>,
        findPinned<NOTATION::COLOR::color::black, 19>,
        findPinned<NOTATION::COLOR::color::black, 20>,
        findPinned<NOTATION::COLOR::color::black, 21>,
        findPinned<NOTATION::COLOR::color::black, 22>,
        findPinned<NOTATION::COLOR::color::black, 23>,
        findPinned<NOTATION::COLOR::color::black, 24>,
        findPinned<NOTATION::COLOR::color::black, 25>,
        findPinned<NOTATION::COLOR::color::black, 26>,
        findPinned<NOTATION::COLOR::color::black, 27>,
        findPinned<NOTATION::COLOR::color::black, 28>,
        findPinned<NOTATION::COLOR::color::black, 29>,
        findPinned<NOTATION::COLOR::color::black, 30>,
        findPinned<NOTATION::COLOR::color::black, 31>,
        findPinned<NOTATION::COLOR::color::black, 32>,
        findPinned<NOTATION::COLOR::color::black, 33>,
        findPinned<NOTATION::COLOR::color::black, 34>,
        findPinned<NOTATION::COLOR::color::black, 35>,
        findPinned<NOTATION::COLOR::color::black, 36>,
        findPinned<NOTATION::COLOR::color::black, 37>,
        findPinned<NOTATION::COLOR::color::black, 38>,
        findPinned<NOTATION::COLOR::color::black, 39>,
        findPinned<NOTATION::COLOR::color::black, 40>,
        findPinned<NOTATION::COLOR::color::black, 41>,
        findPinned<NOTATION::COLOR::color::black, 42>,
        findPinned<NOTATION::COLOR::color::black, 43>,
        findPinned<NOTATION::COLOR::color::black, 44>,
        findPinned<NOTATION::COLOR::color::black, 45>,
        findPinned<NOTATION::COLOR::color::black, 46>,
        findPinned<NOTATION::COLOR::color::black, 47>,
        findPinned<NOTATION::COLOR::color::black, 48>,
        findPinned<NOTATION::COLOR::color::black, 49>,
        findPinned<NOTATION::COLOR::color::black, 50>,
        findPinned<NOTATION::COLOR::color::black, 51>,
        findPinned<NOTATION::COLOR::color::black, 52>,
        findPinned<NOTATION::COLOR::color::black, 53>,
        findPinned<NOTATION::COLOR::color::black, 54>,
        findPinned<NOTATION::COLOR::color::black, 55>,
        findPinned<NOTATION::COLOR::color::black, 56>,
        findPinned<NOTATION::COLOR::color::black, 57>,
        findPinned<NOTATION::COLOR::color::black, 58>,
        findPinned<NOTATION::COLOR::color::black, 59>,
        findPinned<NOTATION::COLOR::color::black, 60>,
        findPinned<NOTATION::COLOR::color::black, 61>,
        findPinned<NOTATION::COLOR::color::black, 62>,
        findPinned<NOTATION::COLOR::color::black, 63>,
};

}

template <NOTATION::COLOR::color c>
uint64_t findPinned(const Board& board, unsigned char kingPos);

template <>
uint64_t findPinned<NOTATION::COLOR::color::white>(const Board& board, unsigned char kingPos)
{
    return whiteLookup[kingPos](board);
}

template <>
uint64_t findPinned<NOTATION::COLOR::color::black>(const Board& board, unsigned char kingPos)
{
    return blackLookup[kingPos](board);
}