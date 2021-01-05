#pragma once

namespace NOTATION
{


namespace PIECE_FEATURES
{

constexpr unsigned char CAN_ATTACK_ON_DIAGONAL = (1u << 2);
constexpr unsigned char CAN_ATTACK_ON_LINES = (1u << 3);

}

namespace PIECES
{
constexpr unsigned char PAWN = (1u << 4);
constexpr unsigned char ROCK = PIECE_FEATURES::CAN_ATTACK_ON_LINES;
constexpr unsigned char KNIGHT = (2u << 4);
constexpr unsigned char BISHOP = PIECE_FEATURES::CAN_ATTACK_ON_DIAGONAL;
constexpr unsigned char QUEEN = PIECE_FEATURES::CAN_ATTACK_ON_LINES | PIECE_FEATURES::CAN_ATTACK_ON_DIAGONAL;
constexpr unsigned char KING = (3u << 4);
constexpr unsigned char PIECES_MASK = PAWN | ROCK | KNIGHT | BISHOP | QUEEN | KING;
}  // namespace PIECES

namespace COLOR
{
constexpr unsigned char WHITE = (0b01);
constexpr unsigned char BLACK = (0b10);
constexpr unsigned char COLOR_MASK = WHITE | BLACK;

enum class color : unsigned char {
	white = WHITE, 
	black = BLACK
};
}  // namespace COLOR

namespace MOVED
{
// TODO:  To consider different approach. This can affect hashing.
//        Two same positions may end as different hash.
//        Especially in early stages of the game.
constexpr unsigned char MOVED_MASK = (1u << 7);
}  //namespace MOVED

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

constexpr unsigned char COLOR_AND_PIECE_MASK = PIECES::PIECES_MASK | COLOR::COLOR_MASK;
constexpr unsigned char MOVED_AND_PIECE_MASK = PIECES::PIECES_MASK | MOVED::MOVED_MASK;

}  // namespace NOTATION
