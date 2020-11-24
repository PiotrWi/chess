#include "Board.hpp"

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

bool validateMove(const Board, const Move)
{
	// Not implemented
	throw 1;	
}

namespace
{
	namespace COLUMN
	{
		constexpr unsigned char A = 0;
		constexpr unsigned char B = 1;
		constexpr unsigned char C = 2;
		constexpr unsigned char D = 3;
		constexpr unsigned char E = 4;
                constexpr unsigned char F = 5;
                constexpr unsigned char G = 6;
                constexpr unsigned char H = 7;
	}
	unsigned char getColumnNum(unsigned char field)
	{
		return field % 8u;
	}
	unsigned char getRow(unsigned char field)
	{
		return (field >> 3u);
	}
	color switchColor(color c)
	{
		auto colorNum = static_cast<unsigned char>(c);
		return color(++colorNum & COLOR_MASK);
	}
	unsigned getFieldNum(const unsigned char row, const unsigned char col)
	{
		return (row << 3u) | col;
	}
}

void applyMove(Board& board, const Move& move)
{
	bool isCastle = (board[move.source] == KING
		and getColumnNum(move.source) == COLUMN::E
		and (getColumnNum(move.source) == COLUMN::C
			or getColumnNum(move.source) == COLUMN::G));

	board[move.destination] = board[move.source];
	board[move.source] = 0;
		
	if (isCastle)
	{
		auto row = getRow(move.source);
		bool isLongCastle = getColumnNum(move.source) == COLUMN::C;
		if (isLongCastle)
		{
			auto rockSource = getFieldNum(row, COLUMN::A);	
			auto rockDestination = getFieldNum(row, COLUMN::D);
			board[rockDestination] = board[rockSource];
			board[rockSource] = 0u;
		}
		else
		{
                        auto rockSource = getFieldNum(row, COLUMN::A);
                        auto rockDestination = getFieldNum(row, COLUMN::D);
                        board[rockDestination] = board[rockSource];
                        board[rockSource] = 0u;

		}
	}

	board.playerOnMove = switchColor(board.playerOnMove);
}

void initDefault(Board& board)
{
        board["a1"] = WHITE | ROCK;
        board["b1"] = WHITE | KNIGHT;
        board["c1"] = WHITE | BISHOP;
        board["d1"] = WHITE | QUEEN;
        board["e1"] = WHITE | KING;
        board["f1"] = WHITE | BISHOP;
        board["g1"] = WHITE | KNIGHT;
        board["h1"] = WHITE | ROCK;

        char field[3] = "a2";
        for (auto&& c: "abcdefgh")
        {
                field[0] = c;
                board[field] = WHITE | PAWN;
        }

        board["a8"] = BLACK | ROCK;
        board["b8"] = BLACK | KNIGHT;
        board["c8"] = BLACK | BISHOP;
        board["d8"] = BLACK | QUEEN;
        board["e8"] = BLACK | KING;
        board["f8"] = BLACK | BISHOP;
        board["g8"] = BLACK | KNIGHT;
        board["h8"] = BLACK | ROCK;

        field[2] = 7;
        for (auto&& c: "abcdefgh")
        {
                field[0] = c;
                board[field] = BLACK | PAWN;
        }
}

