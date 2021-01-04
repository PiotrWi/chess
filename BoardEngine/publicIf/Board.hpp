#pragma once

#include <iosfwd>
#include <publicIf/Notation.hpp>

struct Move
{
	Move() noexcept;
	explicit Move(const char*, const char*, bool isPromoted = false, unsigned char promoteTo = 0) noexcept;
    explicit Move(unsigned char, unsigned char, bool isPromoted = false, unsigned char promoteTo = 0) noexcept;

	unsigned char source;
	unsigned char destination;
	bool isPromoted;
	unsigned char promoteTo;
};

bool operator==(const Move& lfs, const Move& rhs) noexcept;

struct Board
{
	NOTATION::COLOR::color playerOnMove = NOTATION::COLOR::color::white;
	signed char validEnPassant = -1;
	unsigned char fields[64] = {0u, 0u}; // "0 belongs to A1, 1 belongs to B1, 8 belongs to A2

	unsigned char& operator[](const char*) noexcept;
	unsigned char& operator[](const unsigned char) noexcept;
	const unsigned char& operator[](const char*) const noexcept;
	const unsigned char& operator[](const unsigned char) const noexcept;
};

bool operator==(const Board& lhs, const Board& rhs) noexcept;

void initDefault(Board& board) noexcept;
