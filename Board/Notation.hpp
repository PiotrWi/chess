#pragma once

namespace NOTATION
{
namespace PIECES
{
constexpr unsigned char PAWN = 2u;
constexpr unsigned char ROCK = (2u << 1);
constexpr unsigned char KNIGHT = (2u << 2);
constexpr unsigned char BISHOP = (2u << 3);
constexpr unsigned char QUEEN = (2u << 4);
constexpr unsigned char KING = (2u << 5);
constexpr unsigned char PIECES_MASK = PAWN | ROCK | KNIGHT | BISHOP | QUEEN | KING;
}

namespace COLOR
{
constexpr unsigned char WHITE = (0u);
constexpr unsigned char BLACK = (1u);
constexpr unsigned char COLOR_MASK = WHITE | BLACK;

enum class color : unsigned char {
	white = WHITE, 
	black = BLACK
};
}

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
}
}
}
