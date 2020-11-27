#include "CheckChecker.hpp"

#include <algorithm>
#include <initializer_list>
#include <utility>

#include <Board.hpp>
#include <NotationConversions.hpp>

namespace
{

bool isRowInBoard(signed char row)
{
	return row < 8u and row >= 0;
}
bool isColumnInBoard(signed char col)
{
	return col < 8u and col >=0;
}

NOTATION::COLOR::color getOpositeColor(const Board& board, const unsigned char row, const unsigned char col)
{
	return NotationConversions::switchColor(
			NotationConversions::getPieceColor(board[NotationConversions::getFieldNum(row, col)]));
}

unsigned char getOpositeColorBin(const Board& board, const unsigned char row, const unsigned char col)
{
	return static_cast<unsigned char>(getOpositeColor(board, row, col));
}

std::pair<signed char, signed char> modifyCoordinates(std::pair<unsigned char, unsigned char> co,
	std::pair<signed char, signed char> dir)
{
	co.first += dir.first;
	co.second += dir.second;
	return co;
}

bool isAttackedByOpositePawn(const Board& board,
	unsigned char row,
	unsigned char col)
{
	auto pawnColor = getOpositeColor(board, row, col);

	signed char pawnRow = 0;
	auto pawnMask = NOTATION::PIECES::PAWN;
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

	bool rowInBoard = false;
	signed char leftColumn = col - 1;
	signed char rightColumn = col + 1;

	return isRowInBoard(pawnRow) and
			((isColumnInBoard(leftColumn) and board[NotationConversions::getFieldNum(pawnRow, leftColumn)] == pawnMask)
			or (isColumnInBoard(rightColumn) and board[NotationConversions::getFieldNum(pawnRow, leftColumn)] == pawnMask));
}

bool isAttackedOnDiagonalByOpositeBishopOrQueen(const Board& board, unsigned char row, unsigned char col)
{
	auto colorBin = getOpositeColorBin(board, row, col);

	unsigned char bishopPattern = colorBin | NOTATION::PIECES::BISHOP;
	unsigned char queenPattern = colorBin | NOTATION::PIECES::QUEEN;

	for (const auto& dir: {
		std::pair<signed char, signed char>(1, 1),
		std::pair<signed char, signed char>(-1, -1),
		std::pair<signed char, signed char>(-1, 1),
		std::pair<signed char, signed char>(1, -1)})
	{
		for (auto co = modifyCoordinates(std::make_pair(row, col), dir);
			isRowInBoard(co.first) && isColumnInBoard(co.second);
			co = modifyCoordinates(co, dir))
		{
			const auto& field = board.fields[NotationConversions::getFieldNum(co.first, co.second)];
			if (field != 0u)
			{
				if ((field == bishopPattern) | (field == queenPattern))
				{
					return true;
				}
				break;
			}
		}
	}

	return false;
}

bool isAttackedByRookOrQueen(const Board& board, unsigned char row, unsigned char col)
{
	auto colorBin = getOpositeColorBin(board, row, col);
	unsigned char rockPattern = colorBin | NOTATION::PIECES::ROCK;
	unsigned char gueenPattern = colorBin | NOTATION::PIECES::QUEEN;
	for (const auto& dir: {
		std::pair<signed char, signed char>(1, 0),
		std::pair<signed char, signed char>(0, 1),
		std::pair<signed char, signed char>(-1, 0),
		std::pair<signed char, signed char>(0, -1)})
	{
		for (auto co = modifyCoordinates(std::make_pair(row, col), dir);
			isRowInBoard(co.first) && isColumnInBoard(co.second);
			co = modifyCoordinates(co, dir))
		{
			const auto& field = board.fields[NotationConversions::getFieldNum(co.first, co.second)];
			if (field != 0u)
			{
				if (field == rockPattern or field == gueenPattern)
				{
					return true;
				}
				break;
			}
		}
	}

	return false;
}

bool isAttackedByKing(const Board& board, unsigned char row, unsigned char col)
{
	auto colorBin = getOpositeColorBin(board, row, col);
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
		if (!isRowInBoard(co.first) or !isColumnInBoard(co.second))
		{
			continue;
		}
		const auto& field = board.fields[NotationConversions::getFieldNum(co.first, co.second)];

		if (field == kingPattern)
		{
			return true;
		}
	}

	return false;
}

bool isAttackedByKnight(const Board& board, unsigned char row, unsigned char col)
{
	auto colorBin = getOpositeColorBin(board, row, col);
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
		if (!isRowInBoard(co.first) or !isColumnInBoard(co.second))
		{
					continue;
		}
		const auto& field = board.fields[NotationConversions::getFieldNum(co.first, co.second)];

		if (field == knightPattern)
		{
			return true;
		}
	}

	return false;
}

}

namespace CheckChecker
{

bool isCheckOn(const Board& board, const NOTATION::COLOR::color c)
{
	unsigned char KING_MASQ = static_cast<unsigned char>(c) | NOTATION::PIECES::KING;
	unsigned char kingPos = std::find(board.fields, board.fields + 64, KING_MASQ) - board.fields;

	auto kingRow = NotationConversions::getRow(kingPos);
	auto kingColumn = NotationConversions::getColumnNum(kingPos);

    return isAttackedByOpositePawn(board, kingRow, kingColumn)
    	|| isAttackedOnDiagonalByOpositeBishopOrQueen(board, kingRow, kingColumn)
		|| isAttackedByRookOrQueen(board, kingRow, kingColumn)
		|| isAttackedByKing(board, kingRow, kingColumn)
		|| isAttackedByKnight(board, kingRow, kingColumn);
}

}
