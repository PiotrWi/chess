#pragma once

#include <stdint.h>

namespace NOTATION
{

namespace PIECES
{
constexpr unsigned char PAWN =      0b0010;
constexpr unsigned char ROCK =      0b0100;
constexpr unsigned char KNIGHT =    0b0110;
constexpr unsigned char BISHOP =    0b1000;
constexpr unsigned char QUEEN =     0b1010;
constexpr unsigned char KING =      0b1100;
constexpr unsigned char PIECES_MASK = PAWN | ROCK | KNIGHT | BISHOP | QUEEN | KING;
}  // namespace PIECES

namespace COLOR
{
constexpr unsigned char WHITE = 0b0;
constexpr unsigned char BLACK = 0b1;
constexpr unsigned char COLOR_MASK = WHITE | BLACK;

enum class color : unsigned char {
	white = WHITE, 
	black = BLACK
};
}  // namespace COLOR

namespace COORDINATES
{
namespace COLUMN
{
constexpr unsigned char A = 0;
constexpr unsigned char B = 1;
constexpr unsigned char C = 2;
constexpr unsigned char D = 3;
constexpr unsigned char E = 4;
constexpr unsigned char F = 5;
constexpr unsigned char G = 6;
constexpr unsigned char H = 7;
}  // namespace COLUMN

constexpr unsigned char ROW_DIFF = 8;
}  // namespace COORDINATES

namespace CASTLING_RIGHTS
{
constexpr unsigned char WHITE_LONG_BIT = 1;
constexpr unsigned char WHITE_SHORT_BIT = 2;
constexpr unsigned char BLACK_LONG_BIT = 4;
constexpr unsigned char BLACK_SHORT_BIT = 8;

constexpr unsigned char CASTLING_MASK = WHITE_LONG_BIT | WHITE_SHORT_BIT | BLACK_LONG_BIT | BLACK_SHORT_BIT;
}

}  // namespace NOTATION
