#pragma once

#include <cstddef>
#include "core/Notation.hpp"

namespace NotationConversions
{

constexpr NOTATION::COLOR::color switchColor(const NOTATION::COLOR::color c)
{
    auto colorNum = static_cast<unsigned char>(c);
    return NOTATION::COLOR::color(colorNum ^ NOTATION::COLOR::COLOR_MASK);
}

constexpr unsigned char getColumnNum(unsigned char field)
{
    return field & 0b111;
}

constexpr unsigned char getRow(unsigned char field)
{
    return (field >> 3u);
}

constexpr unsigned char getFieldNum(const unsigned char row, const unsigned char col)
{
    return (row << 3u) | col;
}

constexpr unsigned char getFieldNum(const char* field)
{
    unsigned char collumn = field[0] - 'a';
    unsigned char row = field[1] - '1';
    return row*8u + collumn;
}

unsigned char getPieceType(unsigned char piece);

NOTATION::COLOR::color getPieceColor(unsigned char piece);

bool isRowInBoard(unsigned char row);
bool isColumnInBoard(unsigned char col);
}

constexpr NOTATION::COLOR::color operator+(NOTATION::COLOR::color c, unsigned char)
{
    auto colorNum = static_cast<unsigned char>(c);
    return NOTATION::COLOR::color(colorNum ^ NOTATION::COLOR::COLOR_MASK);
}

NOTATION::COLOR::color operator++(NOTATION::COLOR::color& c);
NOTATION::COLOR::color operator++(NOTATION::COLOR::color& c, int);
bool operator==(const unsigned char, const NOTATION::COLOR::color);

constexpr unsigned char operator "" _field(const char* fieldLiteral, size_t)
{
    return NotationConversions::getFieldNum(fieldLiteral);
}

constexpr uint64_t operator "" _bit(const char* fieldLiteral, size_t)
{
    return (1ull << NotationConversions::getFieldNum(fieldLiteral));
}