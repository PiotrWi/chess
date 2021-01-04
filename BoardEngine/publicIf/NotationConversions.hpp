#pragma once

#include <publicIf/Notation.hpp>

namespace NotationConversions
{

NOTATION::COLOR::color switchColor(const NOTATION::COLOR::color c);

unsigned char getColumnNum(unsigned char field);
unsigned char getRow(unsigned char field);
unsigned char getFieldNum(const unsigned char row, const unsigned char col);
unsigned char getFieldNum(const char* field);

unsigned char getPieceType(unsigned char piece);

NOTATION::COLOR::color getPieceColor(unsigned char piece);

bool isRowInBoard(unsigned char row);
bool isColumnInBoard(unsigned char col);

}

NOTATION::COLOR::color operator+(const NOTATION::COLOR::color& c, unsigned char);
NOTATION::COLOR::color operator++(NOTATION::COLOR::color& c);
NOTATION::COLOR::color operator++(NOTATION::COLOR::color& c, int);
bool operator==(const unsigned char, const NOTATION::COLOR::color);