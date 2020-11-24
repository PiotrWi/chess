#pragma once

#include <Notation.hpp>

class Piece
{
public:
	Piece(char, color);
	const char* getFullName() const;
	const char* getColor() const;
	const char* getPieceName() const;

	~Piece() = default;
private:
	char PieceName_;
	color c_;
};

Piece createPiece(const unsigned char number);
