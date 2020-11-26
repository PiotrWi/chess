#pragma once

#include <Notation.hpp>

struct Move
{
	unsigned char source;
	unsigned char destination;
	bool isPromoted;
	unsigned char promoteTo;
};

struct Board
{
	NOTATION::COLOR::color playerOnMove;
	Move lastMove;
	bool hasCastled[2];
	unsigned char fields[64]; // "0 belongs to A0, 1 belongs to B0, 8 belongs to A1

	unsigned char& operator[](const char*);
	unsigned char& operator[](unsigned char);
	const unsigned char& operator[](const char*) const;
	const unsigned char& operator[](unsigned char) const;
};

bool validateMove(const Board, const Move);
void applyMove(Board&, const Move&);
void initDefault(Board& board);
bool isCheckOn(const Board&, const NOTATION::COLOR::color);
bool isMate();
bool isDraw();
