#pragma once

constexpr unsigned char PAWN = 1u;
constexpr unsigned char ROCK = (1u << 1);
constexpr unsigned char KNIGHT = (1u << 2);
constexpr unsigned char BISHOP = (1u << 3);
constexpr unsigned char QUEEN = (1u << 4);
constexpr unsigned char KING = (1u << 5);
constexpr unsigned char PIECES_MASK = PAWN | ROCK | KNIGHT | BISHOP | QUEEN | KING;

constexpr unsigned char WHITE = (1u << 6);
constexpr unsigned char BLACK = (1u << 7);
constexpr unsigned char COLOR_MASK = WHITE | BLACK;

enum class color {white = WHITE, black = BLACK};
