#pragma once

#include <Notation.hpp>

NOTATION::COLOR::color switchColor(NOTATION::COLOR::color c);

unsigned char getColumnNum(unsigned char field);
unsigned char getRow(unsigned char field);
unsigned getFieldNum(const unsigned char row, const unsigned char col);
