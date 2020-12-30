#include "CheckChecker.hpp"

#include <algorithm>
#include <utility>

#include <publicIf/Board.hpp>
#include <publicIf/NotationConversions.hpp>

namespace
{

std::pair<signed char, signed char> modifyCoordinates(std::pair<unsigned char, unsigned char> co,
	std::pair<signed char, signed char> dir)
{
	co.first += dir.first;
	co.second += dir.second;
	return co;
}

unsigned char getColoredPiece(const Board& board,
	unsigned char row,
	unsigned char col)
{
	return board[NotationConversions::getFieldNum(row, col)] & NOTATION::COLOR_AND_PIECE_MASK;
}

bool isAttackedByOppositePawn(const Board& board,
                              NOTATION::COLOR::color pawnColor,
                              unsigned char row,
                              unsigned char col)
{
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

	signed char leftColumn = col - 1;
	signed char rightColumn = col + 1;

	return NotationConversions::isRowInBoard(pawnRow) and
			((NotationConversions::isColumnInBoard(leftColumn) and getColoredPiece(board, pawnRow, leftColumn) == pawnMask)
			or (NotationConversions::isColumnInBoard(rightColumn) and getColoredPiece(board, pawnRow, rightColumn) == pawnMask));
}

bool isAttackedOnDiagonalByOppositeBishopOrQueen(const Board& board,
	NOTATION::COLOR::color oppositeColor,
	unsigned char row,
	unsigned char col)
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
			const auto& field = getColoredPiece(board, co.first, co.second);
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
	unsigned char col)
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
			const auto& field = getColoredPiece(board, co.first, co.second);
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
		unsigned char col)
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
		const auto& field = getColoredPiece(board, co.first, co.second);

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
		unsigned char col)
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

	    const auto& field = getColoredPiece(board, co.first, co.second);

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

bool isAttackedOn(const Board& board,
		NOTATION::COLOR::color playerColor,
		unsigned char fieldPosition)
{
	auto oppositeColor = ++playerColor;
	auto pRow = NotationConversions::getRow(fieldPosition);
	auto pColumn = NotationConversions::getColumnNum(fieldPosition);

    return isAttackedByOppositePawn(board, oppositeColor, pRow, pColumn)
           || isAttackedOnDiagonalByOppositeBishopOrQueen(board, oppositeColor, pRow, pColumn)
           || isAttackedByRookOrQueen(board, oppositeColor, pRow, pColumn)
           || isAttackedByKing(board, oppositeColor, pRow, pColumn)
           || isAttackedByKnight(board, oppositeColor, pRow, pColumn);
}

unsigned char findKing(const Board& board, const NOTATION::COLOR::color c)
{
    unsigned char KING_MASQ = static_cast<unsigned char>(c) | NOTATION::PIECES::KING;
    unsigned char kingPos = std::find_if(board.fields, board.fields + 64, [&](auto&& field){
        return KING_MASQ == (field & NOTATION::COLOR_AND_PIECE_MASK);
    }) - board.fields;
    return kingPos;
}

bool isCheckOn(const Board& board, const NOTATION::COLOR::color c)
{
	return isAttackedOn(board, c, findKing(board, c));
}

}
