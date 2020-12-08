#pragma once

#include <Notation.hpp>

namespace NotationConversions
{

NOTATION::COLOR::color switchColor(NOTATION::COLOR::color c);

unsigned char getColumnNum(unsigned char field);
unsigned char getRow(unsigned char field);
unsigned char getFieldNum(const unsigned char row, const unsigned char col);
unsigned char getFieldNum(const char* field);

unsigned char getPieceType(unsigned char piece);

NOTATION::COLOR::color getPieceColor(unsigned char piece);

bool isRowInBoard(unsigned char row);
bool isColumnInBoard(unsigned char col);

}
