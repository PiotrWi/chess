#include "CheckChecker.hpp"

#include <algorithm>
#include <publicIf/Board.hpp>
#include <publicIf/NotationConversions.hpp>

#include <FieldLookup/FieldLookup.hpp>

namespace CheckChecker
{

namespace
{
template <NOTATION::COLOR::color color, unsigned char kingPosition>
bool isAttackedOn(const Board& board)
{
    constexpr auto oppositeColorBin = static_cast<unsigned char>(color+1);

    constexpr auto pawnMask = NOTATION::PIECES::PAWN | oppositeColorBin;
    for (const unsigned char* pos = FieldLookup<color, kingPosition>::OppositePawnPositions;
        pos < FieldLookup<color, kingPosition>::OppositePawnPositions + FieldLookup<color, kingPosition>::OppositePawnPositionsSize;
        ++pos)
    {
        if (board.getField(*pos) != 0 and board.getField(*pos) == pawnMask) return true;
    }

    constexpr unsigned char kingPattern = oppositeColorBin | NOTATION::PIECES::KING;
    for (const unsigned char* pos = FieldLookup<color, kingPosition>::KingPossibleMoves;
         pos < FieldLookup<color, kingPosition>::KingPossibleMoves + FieldLookup<color, kingPosition>::KingPossibleMovesSize;
         ++pos)
    {
        if (board.getField(*pos) != 0 and board.getField(*pos) == kingPattern) return true;
    }

    constexpr unsigned char knightPattern = oppositeColorBin | NOTATION::PIECES::KNIGHT;
    for (const unsigned char* pos = FieldLookup<color, kingPosition>::KnightPossibleMoves;
         pos < FieldLookup<color, kingPosition>::KnightPossibleMoves + FieldLookup<color, kingPosition>::KnightPossibleMovesSize;
         ++pos)
    {
        if (board.getField(*pos) != 0 and board.getField(*pos) == knightPattern) return true;
    }

    constexpr unsigned char queenPattern = oppositeColorBin | NOTATION::PIECES::QUEEN;
    constexpr unsigned char bishopPattern = oppositeColorBin | NOTATION::PIECES::BISHOP;
    for (const unsigned char* pos = FieldLookup<color, kingPosition>::TopLeftPossibleMoves;
         pos < FieldLookup<color, kingPosition>::TopLeftPossibleMoves + FieldLookup<color, kingPosition>::TopLeftPossibleMovesSize;
         ++pos)
    {
        if (board.getField(*pos) != 0)
        {
            if (board.getField(*pos)== bishopPattern or board.getField(*pos) == queenPattern)
            {
                return true;
            }
            break;
        }

    }
    for (const unsigned char* pos = FieldLookup<color, kingPosition>::TopRightPossibleMoves;
         pos < FieldLookup<color, kingPosition>::TopRightPossibleMoves + FieldLookup<color, kingPosition>::TopRightPossibleMovesSize;
         ++pos)
    {
        if (board.getField(*pos) != 0)
        {
            if (board.getField(*pos)== bishopPattern or board.getField(*pos) == queenPattern)
            {
                return true;
            }
            break;
        }
    }
    for (const unsigned char* pos = FieldLookup<color, kingPosition>::BottomLeftPossibleMoves;
         pos < FieldLookup<color, kingPosition>::BottomLeftPossibleMoves + FieldLookup<color, kingPosition>::BottomLeftPossibleMovesSize;
         ++pos)
    {
        if (board.getField(*pos) != 0)
        {
            if (board.getField(*pos)== bishopPattern or board.getField(*pos) == queenPattern)
            {
                return true;
            }
            break;
        }
    }
    for (const unsigned char* pos = FieldLookup<color, kingPosition>::BottomRightPossibleMoves;
         pos < FieldLookup<color, kingPosition>::BottomRightPossibleMoves + FieldLookup<color, kingPosition>::BottomRightPossibleMovesSize;
         ++pos)
    {
        if (board.getField(*pos) != 0)
        {
            if (board.getField(*pos)== bishopPattern or board.getField(*pos) == queenPattern)
            {
                return true;
            }
            break;
        }
    }

    constexpr unsigned char rockPattern = oppositeColorBin | NOTATION::PIECES::ROCK;
    for (const unsigned char* pos = FieldLookup<color, kingPosition>::TopPossibleMoves;
         pos < FieldLookup<color, kingPosition>::TopPossibleMoves + FieldLookup<color, kingPosition>::TopPossibleMovesSize;
         ++pos)
    {
        if (board.getField(*pos) != 0)
        {
            if (board.getField(*pos)== rockPattern or board.getField(*pos) == queenPattern)
            {
                return true;
            }
            break;
        }
    }
    for (const unsigned char* pos = FieldLookup<color, kingPosition>::BottomPossibleMoves;
         pos < FieldLookup<color, kingPosition>::BottomPossibleMoves + FieldLookup<color, kingPosition>::BottomPossibleMovesSize;
         ++pos)
    {
        if (board.getField(*pos) != 0)
        {
            if (board.getField(*pos)== rockPattern or board.getField(*pos) == queenPattern)
            {
                return true;
            }
            break;
        }
    }
    for (const unsigned char* pos = FieldLookup<color, kingPosition>::LeftPossibleMoves;
         pos < FieldLookup<color, kingPosition>::LeftPossibleMoves + FieldLookup<color, kingPosition>::LeftPossibleMovesSize;
         ++pos)
    {
        if (board.getField(*pos) != 0)
        {
            if (board.getField(*pos)== rockPattern or board.getField(*pos) == queenPattern)
            {
                return true;
            }
            break;
        }
    }
    for (const unsigned char* pos = FieldLookup<color, kingPosition>::RightPossibleMoves;
         pos < FieldLookup<color, kingPosition>::RightPossibleMoves + FieldLookup<color, kingPosition>::RightPossibleMovesSize;
         ++pos)
    {
        if (board.getField(*pos) != 0)
        {
            if (board.getField(*pos)== rockPattern or board.getField(*pos) == queenPattern)
            {
                return true;
            }
            break;
        }

    }

    return false;
}

static bool(*whiteLookup[])(const Board& board)  = {
        isAttackedOn<NOTATION::COLOR::color::white, 0>,
        isAttackedOn<NOTATION::COLOR::color::white, 1>,
        isAttackedOn<NOTATION::COLOR::color::white, 2>,
        isAttackedOn<NOTATION::COLOR::color::white, 3>,
        isAttackedOn<NOTATION::COLOR::color::white, 4>,
        isAttackedOn<NOTATION::COLOR::color::white, 5>,
        isAttackedOn<NOTATION::COLOR::color::white, 6>,
        isAttackedOn<NOTATION::COLOR::color::white, 7>,
        isAttackedOn<NOTATION::COLOR::color::white, 8>,
        isAttackedOn<NOTATION::COLOR::color::white, 9>,
        isAttackedOn<NOTATION::COLOR::color::white, 10>,
        isAttackedOn<NOTATION::COLOR::color::white, 11>,
        isAttackedOn<NOTATION::COLOR::color::white, 12>,
        isAttackedOn<NOTATION::COLOR::color::white, 13>,
        isAttackedOn<NOTATION::COLOR::color::white, 14>,
        isAttackedOn<NOTATION::COLOR::color::white, 15>,
        isAttackedOn<NOTATION::COLOR::color::white, 16>,
        isAttackedOn<NOTATION::COLOR::color::white, 17>,
        isAttackedOn<NOTATION::COLOR::color::white, 18>,
        isAttackedOn<NOTATION::COLOR::color::white, 19>,
        isAttackedOn<NOTATION::COLOR::color::white, 20>,
        isAttackedOn<NOTATION::COLOR::color::white, 21>,
        isAttackedOn<NOTATION::COLOR::color::white, 22>,
        isAttackedOn<NOTATION::COLOR::color::white, 23>,
        isAttackedOn<NOTATION::COLOR::color::white, 24>,
        isAttackedOn<NOTATION::COLOR::color::white, 25>,
        isAttackedOn<NOTATION::COLOR::color::white, 26>,
        isAttackedOn<NOTATION::COLOR::color::white, 27>,
        isAttackedOn<NOTATION::COLOR::color::white, 28>,
        isAttackedOn<NOTATION::COLOR::color::white, 29>,
        isAttackedOn<NOTATION::COLOR::color::white, 30>,
        isAttackedOn<NOTATION::COLOR::color::white, 31>,
        isAttackedOn<NOTATION::COLOR::color::white, 32>,
        isAttackedOn<NOTATION::COLOR::color::white, 33>,
        isAttackedOn<NOTATION::COLOR::color::white, 34>,
        isAttackedOn<NOTATION::COLOR::color::white, 35>,
        isAttackedOn<NOTATION::COLOR::color::white, 36>,
        isAttackedOn<NOTATION::COLOR::color::white, 37>,
        isAttackedOn<NOTATION::COLOR::color::white, 38>,
        isAttackedOn<NOTATION::COLOR::color::white, 39>,
        isAttackedOn<NOTATION::COLOR::color::white, 40>,
        isAttackedOn<NOTATION::COLOR::color::white, 41>,
        isAttackedOn<NOTATION::COLOR::color::white, 42>,
        isAttackedOn<NOTATION::COLOR::color::white, 43>,
        isAttackedOn<NOTATION::COLOR::color::white, 44>,
        isAttackedOn<NOTATION::COLOR::color::white, 45>,
        isAttackedOn<NOTATION::COLOR::color::white, 46>,
        isAttackedOn<NOTATION::COLOR::color::white, 47>,
        isAttackedOn<NOTATION::COLOR::color::white, 48>,
        isAttackedOn<NOTATION::COLOR::color::white, 49>,
        isAttackedOn<NOTATION::COLOR::color::white, 50>,
        isAttackedOn<NOTATION::COLOR::color::white, 51>,
        isAttackedOn<NOTATION::COLOR::color::white, 52>,
        isAttackedOn<NOTATION::COLOR::color::white, 53>,
        isAttackedOn<NOTATION::COLOR::color::white, 54>,
        isAttackedOn<NOTATION::COLOR::color::white, 55>,
        isAttackedOn<NOTATION::COLOR::color::white, 56>,
        isAttackedOn<NOTATION::COLOR::color::white, 57>,
        isAttackedOn<NOTATION::COLOR::color::white, 58>,
        isAttackedOn<NOTATION::COLOR::color::white, 59>,
        isAttackedOn<NOTATION::COLOR::color::white, 60>,
        isAttackedOn<NOTATION::COLOR::color::white, 61>,
        isAttackedOn<NOTATION::COLOR::color::white, 62>,
        isAttackedOn<NOTATION::COLOR::color::white, 63>,
};


static bool(*blackLookup[])(const Board& board)  = {
        isAttackedOn<NOTATION::COLOR::color::black, 0>,
        isAttackedOn<NOTATION::COLOR::color::black, 1>,
        isAttackedOn<NOTATION::COLOR::color::black, 2>,
        isAttackedOn<NOTATION::COLOR::color::black, 3>,
        isAttackedOn<NOTATION::COLOR::color::black, 4>,
        isAttackedOn<NOTATION::COLOR::color::black, 5>,
        isAttackedOn<NOTATION::COLOR::color::black, 6>,
        isAttackedOn<NOTATION::COLOR::color::black, 7>,
        isAttackedOn<NOTATION::COLOR::color::black, 8>,
        isAttackedOn<NOTATION::COLOR::color::black, 9>,
        isAttackedOn<NOTATION::COLOR::color::black, 10>,
        isAttackedOn<NOTATION::COLOR::color::black, 11>,
        isAttackedOn<NOTATION::COLOR::color::black, 12>,
        isAttackedOn<NOTATION::COLOR::color::black, 13>,
        isAttackedOn<NOTATION::COLOR::color::black, 14>,
        isAttackedOn<NOTATION::COLOR::color::black, 15>,
        isAttackedOn<NOTATION::COLOR::color::black, 16>,
        isAttackedOn<NOTATION::COLOR::color::black, 17>,
        isAttackedOn<NOTATION::COLOR::color::black, 18>,
        isAttackedOn<NOTATION::COLOR::color::black, 19>,
        isAttackedOn<NOTATION::COLOR::color::black, 20>,
        isAttackedOn<NOTATION::COLOR::color::black, 21>,
        isAttackedOn<NOTATION::COLOR::color::black, 22>,
        isAttackedOn<NOTATION::COLOR::color::black, 23>,
        isAttackedOn<NOTATION::COLOR::color::black, 24>,
        isAttackedOn<NOTATION::COLOR::color::black, 25>,
        isAttackedOn<NOTATION::COLOR::color::black, 26>,
        isAttackedOn<NOTATION::COLOR::color::black, 27>,
        isAttackedOn<NOTATION::COLOR::color::black, 28>,
        isAttackedOn<NOTATION::COLOR::color::black, 29>,
        isAttackedOn<NOTATION::COLOR::color::black, 30>,
        isAttackedOn<NOTATION::COLOR::color::black, 31>,
        isAttackedOn<NOTATION::COLOR::color::black, 32>,
        isAttackedOn<NOTATION::COLOR::color::black, 33>,
        isAttackedOn<NOTATION::COLOR::color::black, 34>,
        isAttackedOn<NOTATION::COLOR::color::black, 35>,
        isAttackedOn<NOTATION::COLOR::color::black, 36>,
        isAttackedOn<NOTATION::COLOR::color::black, 37>,
        isAttackedOn<NOTATION::COLOR::color::black, 38>,
        isAttackedOn<NOTATION::COLOR::color::black, 39>,
        isAttackedOn<NOTATION::COLOR::color::black, 40>,
        isAttackedOn<NOTATION::COLOR::color::black, 41>,
        isAttackedOn<NOTATION::COLOR::color::black, 42>,
        isAttackedOn<NOTATION::COLOR::color::black, 43>,
        isAttackedOn<NOTATION::COLOR::color::black, 44>,
        isAttackedOn<NOTATION::COLOR::color::black, 45>,
        isAttackedOn<NOTATION::COLOR::color::black, 46>,
        isAttackedOn<NOTATION::COLOR::color::black, 47>,
        isAttackedOn<NOTATION::COLOR::color::black, 48>,
        isAttackedOn<NOTATION::COLOR::color::black, 49>,
        isAttackedOn<NOTATION::COLOR::color::black, 50>,
        isAttackedOn<NOTATION::COLOR::color::black, 51>,
        isAttackedOn<NOTATION::COLOR::color::black, 52>,
        isAttackedOn<NOTATION::COLOR::color::black, 53>,
        isAttackedOn<NOTATION::COLOR::color::black, 54>,
        isAttackedOn<NOTATION::COLOR::color::black, 55>,
        isAttackedOn<NOTATION::COLOR::color::black, 56>,
        isAttackedOn<NOTATION::COLOR::color::black, 57>,
        isAttackedOn<NOTATION::COLOR::color::black, 58>,
        isAttackedOn<NOTATION::COLOR::color::black, 59>,
        isAttackedOn<NOTATION::COLOR::color::black, 60>,
        isAttackedOn<NOTATION::COLOR::color::black, 61>,
        isAttackedOn<NOTATION::COLOR::color::black, 62>,
        isAttackedOn<NOTATION::COLOR::color::black, 63>,
};

}  // namespace

bool isAttackedOn(const Board& board,
		NOTATION::COLOR::color playerColor,
		unsigned char fieldPosition) noexcept
{
    if (playerColor == NOTATION::COLOR::color::white)
    {
        return whiteLookup[fieldPosition](board);
    }
    else
    {
        return blackLookup[fieldPosition](board);
    }
}

unsigned char findKing(const Board& board, const NOTATION::COLOR::color c) noexcept
{
    return 63 - __builtin_clzll(board.piecesBitSets[static_cast<unsigned char>(c)].kingsMask);
}

bool isCheckOn(const Board& board, const NOTATION::COLOR::color c) noexcept
{
    unsigned char kingPos = 63 - __builtin_clzll(board.piecesBitSets[static_cast<unsigned char>(c)].kingsMask);

    return isAttackedOn(board, c, kingPos);
}

}
