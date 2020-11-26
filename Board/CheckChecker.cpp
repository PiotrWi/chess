#include "CheckChecker.hpp"

#include <algorithm>
#include <initializer_list>
#include <utility>

#include <Board.hpp>
#include <NotationConversions.hpp>

namespace
{

bool isRowInBoard(unsigned char row)
{
	return row < 8u;
}
bool isColumnInBoard(unsigned char col)
{
	return col < 8u;
}

NOTATION::COLOR::color getOpositeColor(const Board& board, const unsigned char row, const unsigned char col)
{
	return switchColor(getPieceColor(board[getFieldNum(row, col)]));
}

unsigned char getOpositeColorBin(const Board& board, const unsigned char row, const unsigned char col)
{
	return static_cast<unsigned char>(getOpositeColor(board, row, col));
}

std::pair<unsigned char, unsigned char> modifyCoordinates(std::pair<unsigned char, unsigned char> co,
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

	auto pawnRow = 0;
	auto pawnMask = NOTATION::PIECES::PAWN;
	if (pawnColor == NOTATION::COLOR::color::white)
	{
		pawnRow = row+1;
		pawnMask |= NOTATION::COLOR::WHITE;
	}
	if (pawnColor == NOTATION::COLOR::color::black)
	{
		pawnRow = row-1;
		pawnMask |= NOTATION::COLOR::BLACK;
	}

	bool rowInBoard = false;
	auto leftColumn = col - 1;
	auto rightColumn = col + 1;
	return isRowInBoard(pawnRow) and
		((isColumnInBoard(leftColumn) and board[getFieldNum(pawnRow, leftColumn)] == pawnMask)
		or (isColumnInBoard(rightColumn) and board[getFieldNum(pawnRow, leftColumn)] == pawnMask));
}

bool isAttackedOnDiagonalByOpositeBishopOrQueen(const Board& board, unsigned char row, unsigned char col)
{
	auto colorBin = getOpositeColorBin(board, row, col);

	auto bishopPattern = colorBin | NOTATION::PIECES::BISHOP;
	auto queenPattern = colorBin | NOTATION::PIECES::QUEEN;

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
			const auto& field = board.fields[getFieldNum(co.first, co.second)];
			if (field != 0u)
			{
				if ((field == bishopPattern) | (field == queenPattern))
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool isAttackedByRookOrQueen(const Board& board, unsigned char row, unsigned char col)
{
	auto colorBin = getOpositeColorBin(board, row, col);
	auto rockPattern = colorBin | NOTATION::PIECES::ROCK;

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
			const auto& field = board.fields[getFieldNum(co.first, co.second)];
			if (field != 0u)
			{
				if (field == rockPattern)
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool isAttackedByKing(const Board& board, unsigned char row, unsigned char col)
{
	auto colorBin = getOpositeColorBin(board, row, col);
	auto kingPattern = colorBin | NOTATION::PIECES::KING;

	for (const auto& adjustmentField: {
		std::pair<signed char, signed char>(1, 1),
		std::pair<signed char, signed char>(1, 0),
		std::pair<signed char, signed char>(1, -1),
		std::pair<signed char, signed char>(0, 1),
		std::pair<signed char, signed char>(1, -1),
		std::pair<signed char, signed char>(-1, -1),
		std::pair<signed char, signed char>(-1, 0),
		std::pair<signed char, signed char>(-1, 0)})
	{
		auto co = modifyCoordinates(std::make_pair(row, col), adjustmentField);
		const auto& field = board.fields[getFieldNum(co.first, co.second)];

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
	auto knightPattern = colorBin | NOTATION::PIECES::KNIGHT;

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
		const auto& field = board.fields[getFieldNum(co.first, co.second)];

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

	auto kingRow = getRow(kingPos);
	auto kingColumn = getColumnNum(kingPos);

    return isAttackedByOpositePawn(board, kingRow, kingColumn)
    	|| isAttackedOnDiagonalByOpositeBishopOrQueen(board, kingRow, kingColumn)
		|| isAttackedByRookOrQueen(board, kingRow, kingColumn)
		|| isAttackedByKing(board, kingRow, kingColumn)
		|| isAttackedByKnight(board, kingRow, kingColumn);
}

}
