#pragma once

#include "core/Notation.hpp"

class Piece
{
public:
	Piece(char, NOTATION::COLOR::color);
	Piece(unsigned char);
	const char* getFullName() const;
	const char* getColor() const;
	const char* getPieceName() const;

	~Piece() = default;
private:
	char PieceName_;
	NOTATION::COLOR::color c_;
};

Piece createPiece(const unsigned char number);
