#pragma once

#include <iosfwd>
#include <Notation.hpp>

struct Move
{
	Move();
	Move(const char*, const char*, bool isPromoted = false, unsigned char promoteTo = 0);
	Move(unsigned char, unsigned char, bool isPromoted = false, unsigned char promoteTo = 0);

	unsigned char source;
	unsigned char destination;
	bool isPromoted;
	unsigned char promoteTo;
};

std::ostream& operator<<(std::ostream&, const Move&);
bool operator==(const Move& lfs, const Move& rhs);

struct Board
{
	NOTATION::COLOR::color playerOnMove = NOTATION::COLOR::color::white;
	Move lastMove = {};
	unsigned char fields[64] = {0u, 0u}; // "0 belongs to A0, 1 belongs to B0, 8 belongs to A1

	unsigned char& operator[](const char*);
	unsigned char& operator[](unsigned char);
	const unsigned char& operator[](const char*) const;
	const unsigned char& operator[](unsigned char) const;
};

bool operator==(const Board& lhs, const Board& rhs);

void initDefault(Board& board);
