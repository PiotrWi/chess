#include "Board.hpp"

#include <algorithm>
#include <initializer_list>

#include <NotationConversions.hpp>
#include <CheckChecker.hpp>

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



bool isCheckOn(const Board& board, const NOTATION::COLOR::color c)
{
	return CheckChecker::isCheckOn(board, c);
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
