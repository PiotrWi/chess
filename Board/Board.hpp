#pragma once

#include <Notation.hpp>

struct Board
{
	color playerOnMove;
	unsigned char fields[64]; // "0 belongs to A0, 1 belongs to B0, 8 belongs to A1

	unsigned char& operator[](const char*);
	unsigned char& operator[](unsigned char);
};

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
