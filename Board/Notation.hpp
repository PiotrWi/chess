#pragma once

constexpr unsigned char PAWN = 2u;
constexpr unsigned char ROCK = (2u << 1);
constexpr unsigned char KNIGHT = (2u << 2);
constexpr unsigned char BISHOP = (2u << 3);
constexpr unsigned char QUEEN = (2u << 4);
constexpr unsigned char KING = (2u << 5);
constexpr unsigned char PIECES_MASK = PAWN | ROCK | KNIGHT | BISHOP | QUEEN | KING;

constexpr unsigned char WHITE = (0u);
constexpr unsigned char BLACK = (1u);
constexpr unsigned char COLOR_MASK = WHITE | BLACK;

enum class color {
	white = WHITE, 
	black = BLACK
};
