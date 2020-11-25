#include "Board.hpp"

#include <algorithm>
#include <initializer_list>

#include <NotationConversions.hpp>

unsigned char& Board::operator[](const char* field)
{
        unsigned char collumn = field[0] - 'a';
        unsigned char row = field[0] - '1';

        return fields[row*8u + collumn];
}

unsigned char& Board::operator[](const unsigned char field)
{
        return fields[field];
}

const unsigned char& Board::operator[](const char* field) const
{
	return const_cast<Board&>(*this)[field];
}

const unsigned char& Board::operator[](const unsigned char field) const
{
	return const_cast<Board&>(*this)[field];
}

bool validateMove(const Board&, const Move)
{
	// Not implemented
	throw 1;
}

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
	return switchColor(static_cast<NOTATION::COLOR::color>(
		board[getFieldNum(row, col)] & NOTATION::COLOR::COLOR_MASK));
}

unsigned char getOpositeColorBin(const Board& board, const unsigned char row, const unsigned char col)
{
	return ((board[getFieldNum(row, col)] & NOTATION::COLOR::COLOR_MASK) + 1u) & NOTATION::COLOR::COLOR_MASK;
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

namespace
{

std::pair<unsigned char, unsigned char> modifyCoordinates(std::pair<unsigned char, unsigned char> co,
	std::pair<signed char, signed char> dir)
{
	co.first += dir.first;
	co.second += dir.second;
	return co;
}

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

bool isAttackedByRookOrQueen(const Board& board, unsigned char row, unsigned char col);
bool isAttackedByKing(const Board& board, unsigned char row, unsigned char col);
bool isAttackedByKnight(const Board& board, unsigned char row, unsigned char col);

}

bool isCheckOn(const Board& board, const NOTATION::COLOR::color c)
{
	unsigned char KING_MASQ = static_cast<unsigned char>(c) | NOTATION::PIECES::KING;
	unsigned char kingPos = std::find(board.fields, board.fields + 64, KING_MASQ) - board.fields;

	auto kingRow = getRow(kingPos);
	auto kingColumn = getColumnNum(kingPos);

	throw 1;
}

void applyMove(Board& board, const Move& move)
{
	bool isCastle = (board[move.source] == NOTATION::PIECES::KING
		and getColumnNum(move.source) == NOTATION::COORDINATES::COLUMN::E
		and (getColumnNum(move.source) == NOTATION::COORDINATES::COLUMN::C
			or getColumnNum(move.source) == NOTATION::COORDINATES::COLUMN::G));

	board[move.destination] = board[move.source];
	board[move.source] = 0;
		
	if (isCastle)
	{
		auto row = getRow(move.source);
		bool isLongCastle = getColumnNum(move.source) == NOTATION::COORDINATES::COLUMN::C;
		if (isLongCastle)
		{
			auto rockSource = getFieldNum(row, NOTATION::COORDINATES::COLUMN::A);
			auto rockDestination = getFieldNum(row, NOTATION::COORDINATES::COLUMN::D);
			board[rockDestination] = board[rockSource];
			board[rockSource] = 0u;
		}
		else
		{
            auto rockSource = getFieldNum(row, NOTATION::COORDINATES::COLUMN::A);
            auto rockDestination = getFieldNum(row, NOTATION::COORDINATES::COLUMN::D);
            board[rockDestination] = board[rockSource];
            board[rockSource] = 0u;

		}
	}

	board.playerOnMove = switchColor(board.playerOnMove);
}

void initDefault(Board& board)
{
        board["a1"] = NOTATION::COLOR::WHITE | NOTATION::PIECES::ROCK;
        board["b1"] = NOTATION::COLOR::WHITE | NOTATION::PIECES::KNIGHT;
        board["c1"] = NOTATION::COLOR::WHITE | NOTATION::PIECES::BISHOP;
        board["d1"] = NOTATION::COLOR::WHITE | NOTATION::PIECES::QUEEN;
        board["e1"] = NOTATION::COLOR::WHITE | NOTATION::PIECES::KING;
        board["f1"] = NOTATION::COLOR::WHITE | NOTATION::PIECES::BISHOP;
        board["g1"] = NOTATION::COLOR::WHITE | NOTATION::PIECES::KNIGHT;
        board["h1"] = NOTATION::COLOR::WHITE | NOTATION::PIECES::ROCK;

        char field[3] = "a2";
        for (auto&& c: {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'})
        {
                field[0] = c;
                board[field] = NOTATION::COLOR::WHITE | NOTATION::PIECES::PAWN;
        }

        board["a8"] = NOTATION::COLOR::BLACK | NOTATION::PIECES::ROCK;
        board["b8"] = NOTATION::COLOR::BLACK | NOTATION::PIECES::KNIGHT;
        board["c8"] = NOTATION::COLOR::BLACK | NOTATION::PIECES::BISHOP;
        board["d8"] = NOTATION::COLOR::BLACK | NOTATION::PIECES::QUEEN;
        board["e8"] = NOTATION::COLOR::BLACK | NOTATION::PIECES::KING;
        board["f8"] = NOTATION::COLOR::BLACK | NOTATION::PIECES::BISHOP;
        board["g8"] = NOTATION::COLOR::BLACK | NOTATION::PIECES::KNIGHT;
        board["h8"] = NOTATION::COLOR::BLACK | NOTATION::PIECES::ROCK;

        field[2] = 7;
        for (auto&& c: {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'})
        {
                field[0] = c;
                board[field] = NOTATION::COLOR::BLACK | NOTATION::PIECES::PAWN;
        }
}
