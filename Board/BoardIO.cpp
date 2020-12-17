#include "BoardIO.hpp"

#include <map>
#include <ostream>

namespace
{

std::map<unsigned char, const char*> symbolToPieceMapping = {
	std::make_pair(0u, " "),
	std::make_pair(NOTATION::COLOR::BLACK | NOTATION::PIECES::ROCK, "♜"),
	std::make_pair(NOTATION::COLOR::BLACK | NOTATION::PIECES::KNIGHT, "♞"),
	std::make_pair(NOTATION::COLOR::BLACK | NOTATION::PIECES::BISHOP, "♝"),
	std::make_pair(NOTATION::COLOR::BLACK | NOTATION::PIECES::QUEEN, "♛"),
	std::make_pair(NOTATION::COLOR::BLACK | NOTATION::PIECES::KING, "♚"),
	std::make_pair(NOTATION::COLOR::BLACK | NOTATION::PIECES::PAWN, "♟"),
	std::make_pair(NOTATION::COLOR::WHITE | NOTATION::PIECES::PAWN, "♙"),
	std::make_pair(NOTATION::COLOR::WHITE | NOTATION::PIECES::QUEEN, "♕"),
	std::make_pair(NOTATION::COLOR::WHITE | NOTATION::PIECES::KING, "♔"),
	std::make_pair(NOTATION::COLOR::WHITE | NOTATION::PIECES::BISHOP, "♗"),
	std::make_pair(NOTATION::COLOR::WHITE | NOTATION::PIECES::KNIGHT, "♘"),
	std::make_pair(NOTATION::COLOR::WHITE | NOTATION::PIECES::ROCK ,"♖"),
};

}  // namespace

std::ostream& operator<<(std::ostream& os, const Board& board)
{
	for (auto row = 56; row >= 0; row -= 8)
	{
		for (auto col = 0; col < 8; ++col)
		{
			os << symbolToPieceMapping[(board[row+col]
				& NOTATION::COLOR_AND_PIECE_MASK)];

		}
		os << std::endl;
	}
	return os;
}

