#include "CheckChecker.hpp"

#include <algorithm>
#include <utility>

#include <publicIf/Board.hpp>
#include <publicIf/NotationConversions.hpp>

#include <FieldLookup/FieldLookup.hpp>

namespace
{

std::pair<signed char, signed char> modifyCoordinates(std::pair<unsigned char, unsigned char> co,
	std::pair<signed char, signed char> dir) noexcept
{
	co.first += dir.first;
	co.second += dir.second;
	return co;
}

bool isAttackedByOppositePawn(const Board& board,
                              NOTATION::COLOR::color pawnColor,
                              unsigned char row,
                              unsigned char col) noexcept
{
	unsigned char pawnRow = 0;
	auto pawnMask = NOTATION::PIECES::PAWN;
    unsigned char leftColumn = col - 1;
    unsigned char rightColumn = col + 1;

	if (pawnColor == NOTATION::COLOR::color::white)
	{
		pawnRow = row-1;
		pawnMask |= NOTATION::COLOR::WHITE;
	}
	if (pawnColor == NOTATION::COLOR::color::black)
	{
		pawnRow = row+1;
		pawnMask |= NOTATION::COLOR::BLACK;
	}

	return NotationConversions::isRowInBoard(pawnRow) and
			((NotationConversions::isColumnInBoard(leftColumn) and board[NotationConversions::getFieldNum(pawnRow, leftColumn)] == pawnMask)
			or (NotationConversions::isColumnInBoard(rightColumn) and board[NotationConversions::getFieldNum(pawnRow, rightColumn)] == pawnMask));
}

bool isAttackedOnDiagonalByOppositeBishopOrQueen(const Board& board,
	NOTATION::COLOR::color oppositeColor,
	unsigned char row,
	unsigned char col) noexcept
{
	auto colorBin = static_cast<unsigned char>(oppositeColor);

	unsigned char pattern = colorBin | NOTATION::PIECE_FEATURES::CAN_ATTACK_ON_DIAGONAL;

	for (const auto& dir: {
		std::pair<signed char, signed char>(1, 1),
		std::pair<signed char, signed char>(-1, -1),
		std::pair<signed char, signed char>(-1, 1),
		std::pair<signed char, signed char>(1, -1)})
	{
		for (auto co = modifyCoordinates(std::make_pair(row, col), dir);
			NotationConversions::isRowInBoard(co.first) && NotationConversions::isColumnInBoard(co.second);
			co = modifyCoordinates(co, dir))
		{
			const auto& field = board[NotationConversions::getFieldNum(co.first, co.second)];
			if (field != 0u)
			{
				if ((field & pattern) == pattern)
				{
					return true;
				}
				break;
			}
		}
	}

	return false;
}

bool isAttackedByRookOrQueen(const Board& board,
	NOTATION::COLOR::color opositeColor,
	unsigned char row,
	unsigned char col) noexcept
{
	auto colorBin = static_cast<unsigned char>(opositeColor);

	unsigned char pattern = colorBin | NOTATION::PIECE_FEATURES::CAN_ATTACK_ON_LINES;

	for (const auto& dir: {
		std::pair<signed char, signed char>(1, 0),
		std::pair<signed char, signed char>(0, 1),
		std::pair<signed char, signed char>(-1, 0),
		std::pair<signed char, signed char>(0, -1)})
	{
		for (auto co = modifyCoordinates(std::make_pair(row, col), dir);
			NotationConversions::isRowInBoard(co.first) && NotationConversions::isColumnInBoard(co.second);
			co = modifyCoordinates(co, dir))
		{
			const auto& field = board[NotationConversions::getFieldNum(co.first, co.second)];
			if (field != 0u)
			{
				if ((field & pattern) == pattern)
				{
					return true;
				}
				break;
			}
		}
	}

	return false;
}

bool isAttackedByKing(const Board& board,
		NOTATION::COLOR::color opositeColor,
		unsigned char row,
		unsigned char col) noexcept
{
	auto colorBin = static_cast<unsigned char>(opositeColor);
	unsigned char kingPattern = colorBin | NOTATION::PIECES::KING;

	for (const auto& adjustmentField: {
		std::pair<signed char, signed char>(1, 1),
		std::pair<signed char, signed char>(1, 0),
		std::pair<signed char, signed char>(1, -1),
		std::pair<signed char, signed char>(0, -1),
		std::pair<signed char, signed char>(0, 1),
		std::pair<signed char, signed char>(-1, -1),
		std::pair<signed char, signed char>(-1, 1),
		std::pair<signed char, signed char>(-1, 0)})
	{
		auto co = modifyCoordinates(std::make_pair(row, col), adjustmentField);
		if (!NotationConversions::isRowInBoard(co.first) or !NotationConversions::isColumnInBoard(co.second))
		{
			continue;
		}
		const auto& field = board[NotationConversions::getFieldNum(co.first, co.second)];

		if (field == kingPattern)
		{
			return true;
		}
	}

	return false;
}

bool isAttackedByKnight(const Board& board,
		NOTATION::COLOR::color opositeColor,
		unsigned char row,
		unsigned char col) noexcept
{
	auto colorBin = static_cast<unsigned char>(opositeColor);
	unsigned char  knightPattern = colorBin | NOTATION::PIECES::KNIGHT;

	for (const auto& adjustmentField: {
		std::pair<signed char, signed char>(2, 1),
		std::pair<signed char, signed char>(2, -1),
		std::pair<signed char, signed char>(1, -2),
		std::pair<signed char, signed char>(1, 2),
		std::pair<signed char, signed char>(-1, -2),
		std::pair<signed char, signed char>(-1, 2),
		std::pair<signed char, signed char>(-2, -1),
		std::pair<signed char, signed char>(-2, 1)})
	{
		auto co = modifyCoordinates(std::make_pair(row, col), adjustmentField);
		if (!NotationConversions::isRowInBoard(co.first) or !NotationConversions::isColumnInBoard(co.second))
		    continue;

	    const auto& field = board[NotationConversions::getFieldNum(co.first, co.second)];

		if (field == knightPattern)
		{
			return true;
		}
	}

	return false;
}

}  // namespace

namespace CheckChecker
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
        if (board[*pos] == pawnMask) return true;
    }

    constexpr unsigned char kingPattern = oppositeColorBin | NOTATION::PIECES::KING;
    for (const unsigned char* pos = FieldLookup<color, kingPosition>::KingPossibleMoves;
         pos < FieldLookup<color, kingPosition>::KingPossibleMoves + FieldLookup<color, kingPosition>::KingPossibleMovesSize;
         ++pos)
    {
        if (board[*pos] == kingPattern) return true;
    }

    constexpr unsigned char knightPattern = oppositeColorBin | NOTATION::PIECES::KNIGHT;
    for (const unsigned char* pos = FieldLookup<color, kingPosition>::KnightPossibleMoves;
         pos < FieldLookup<color, kingPosition>::KnightPossibleMoves + FieldLookup<color, kingPosition>::KnightPossibleMovesSize;
         ++pos)
    {
        if (board[*pos] == knightPattern) return true;
    }

    constexpr unsigned char bishopPattern = oppositeColorBin | NOTATION::PIECE_FEATURES::CAN_ATTACK_ON_DIAGONAL;
    for (const unsigned char* pos = FieldLookup<color, kingPosition>::TopLeftPossibleMoves;
         pos < FieldLookup<color, kingPosition>::TopLeftPossibleMoves + FieldLookup<color, kingPosition>::TopLeftPossibleMovesSize;
         ++pos)
    {
        if ((board[*pos] & bishopPattern) == bishopPattern) return true;
        if (board[*pos] != 0) break;
    }
    for (const unsigned char* pos = FieldLookup<color, kingPosition>::TopRightPossibleMoves;
         pos < FieldLookup<color, kingPosition>::TopRightPossibleMoves + FieldLookup<color, kingPosition>::TopRightPossibleMovesSize;
         ++pos)
    {
        if ((board[*pos] & bishopPattern) == bishopPattern) return true;
        if (board[*pos] != 0) break;
    }
    for (const unsigned char* pos = FieldLookup<color, kingPosition>::BottomLeftPossibleMoves;
         pos < FieldLookup<color, kingPosition>::BottomLeftPossibleMoves + FieldLookup<color, kingPosition>::BottomLeftPossibleMovesSize;
         ++pos)
    {
        if ((board[*pos] & bishopPattern) == bishopPattern) return true;
        if (board[*pos] != 0) break;
    }
    for (const unsigned char* pos = FieldLookup<color, kingPosition>::BottomRightPossibleMoves;
         pos < FieldLookup<color, kingPosition>::BottomRightPossibleMoves + FieldLookup<color, kingPosition>::BottomRightPossibleMovesSize;
         ++pos)
    {
        if ((board[*pos] & bishopPattern) == bishopPattern) return true;
        if (board[*pos] != 0) break;
    }

    constexpr unsigned char rockPattern = oppositeColorBin | NOTATION::PIECE_FEATURES::CAN_ATTACK_ON_LINES;
    for (const unsigned char* pos = FieldLookup<color, kingPosition>::TopPossibleMoves;
         pos < FieldLookup<color, kingPosition>::TopPossibleMoves + FieldLookup<color, kingPosition>::TopPossibleMovesSize;
         ++pos)
    {
        if ((board[*pos] & rockPattern) == rockPattern) return true;
        if (board[*pos] != 0) break;
    }
    for (const unsigned char* pos = FieldLookup<color, kingPosition>::BottomPossibleMoves;
         pos < FieldLookup<color, kingPosition>::BottomPossibleMoves + FieldLookup<color, kingPosition>::BottomPossibleMovesSize;
         ++pos)
    {
        if ((board[*pos] & rockPattern) == rockPattern) return true;
        if (board[*pos] != 0) break;
    }
    for (const unsigned char* pos = FieldLookup<color, kingPosition>::LeftPossibleMoves;
         pos < FieldLookup<color, kingPosition>::LeftPossibleMoves + FieldLookup<color, kingPosition>::LeftPossibleMovesSize;
         ++pos)
    {
        if ((board[*pos] & rockPattern) == rockPattern) return true;
        if (board[*pos] != 0) break;
    }
    for (const unsigned char* pos = FieldLookup<color, kingPosition>::RightPossibleMoves;
         pos < FieldLookup<color, kingPosition>::RightPossibleMoves + FieldLookup<color, kingPosition>::RightPossibleMovesSize;
         ++pos)
    {
        if ((board[*pos] & rockPattern) == rockPattern) return true;
        if (board[*pos] != 0) break;
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

bool isAttackedOnRapid(const Board& board, NOTATION::COLOR::color c, unsigned char fieldPosition)
{
    if (c == NOTATION::COLOR::color::white)
    {
        return whiteLookup[fieldPosition](board);
    }
    else
    {
        return blackLookup[fieldPosition](board);
    }
}

bool isAttackedOn(const Board& board,
		NOTATION::COLOR::color playerColor,
		unsigned char fieldPosition) noexcept
{
    return isAttackedOnRapid(board, playerColor, fieldPosition);
	auto oppositeColor = ++playerColor;
	auto pRow = NotationConversions::getRow(fieldPosition);
	auto pColumn = NotationConversions::getColumnNum(fieldPosition);

    return isAttackedByOppositePawn(board, oppositeColor, pRow, pColumn)
           || isAttackedOnDiagonalByOppositeBishopOrQueen(board, oppositeColor, pRow, pColumn)
           || isAttackedByRookOrQueen(board, oppositeColor, pRow, pColumn)
           || isAttackedByKing(board, oppositeColor, pRow, pColumn)
           || isAttackedByKnight(board, oppositeColor, pRow, pColumn);
}

unsigned char findKing(const Board& board, const NOTATION::COLOR::color c) noexcept
{
    unsigned char KING_MASQ = static_cast<unsigned char>(c) | NOTATION::PIECES::KING;
    unsigned char kingPos = std::find(board.fields, board.fields + 64, KING_MASQ) - board.fields;
    return kingPos;
}

bool isCheckOn(const Board& board, const NOTATION::COLOR::color c) noexcept
{
    unsigned char KING_MASQ = static_cast<unsigned char>(c) | NOTATION::PIECES::KING;
    unsigned char kingPos = std::find(board.fields, board.fields + 64, KING_MASQ) - board.fields;

    return isAttackedOn(board, c, kingPos);
}

}
