#include <gtest/gtest.h>

#include <cstring>
#include <utility>
#include <vector>

#include <Board.hpp>
#include <CheckChecker.hpp>

std::vector<std::pair<const char*, unsigned char>> symbolToPieceMapping = {
	std::make_pair("♜", NOTATION::COLOR::BLACK | NOTATION::PIECES::ROCK),
	std::make_pair("♞", NOTATION::COLOR::BLACK | NOTATION::PIECES::ROCK),
	std::make_pair("♝", NOTATION::COLOR::BLACK | NOTATION::PIECES::ROCK),
	std::make_pair("♛", NOTATION::COLOR::BLACK | NOTATION::PIECES::ROCK),
	std::make_pair("♚", NOTATION::COLOR::BLACK | NOTATION::PIECES::ROCK),
	std::make_pair("♟", NOTATION::COLOR::BLACK | NOTATION::PIECES::ROCK),
	std::make_pair("♙", NOTATION::COLOR::BLACK | NOTATION::PIECES::ROCK),
	std::make_pair("♕", NOTATION::COLOR::BLACK | NOTATION::PIECES::ROCK),
	std::make_pair("♔", NOTATION::COLOR::BLACK | NOTATION::PIECES::ROCK),
	std::make_pair("♗", NOTATION::COLOR::BLACK | NOTATION::PIECES::ROCK),
	std::make_pair("♘", NOTATION::COLOR::BLACK | NOTATION::PIECES::ROCK),
	std::make_pair("♖", NOTATION::COLOR::BLACK | NOTATION::PIECES::ROCK),
};

int unicodeLen(unsigned char c)
{
	if (~c & 0b1000000)
		return 1;
	if ((c & 0b11110000) == 0b11110000)
		return 4;
	if ((c & 0b11100000) == 0b11100000)
		return 3;
	if ((c & 0b11000000) == 0b11000000)
		return 2;
	return 1;
}

unsigned char getCorrespondingFigure(const char* str, int bytesLen)
{
	return 0;
}

void initBoardByString(Board& board,
		const char* position,
		NOTATION::COLOR::color playerOnMove = NOTATION::COLOR::color::white,
		Move lastMove = {})
{
	std::cout << strlen(position) << std::endl;

	for (int str_i = 0, i = 0; str_i < strlen(position); ++i)
	{
		auto len = unicodeLen(position[str_i]);
		if (len != 1)
		{
			getCorrespondingFigure(&position[str_i], len);
		}
		str_i += len;
	}
}

TEST(CheckCheckershould, FindCheckByPawn)
{
	Board board;
	auto position =
			"        "
			"   ♚    "
			"  ♙     "
			"        "
			"        "
			"        "
			"        "
			"    ♔   ";
	initBoardByString (board, position);
	ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));
	ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));
}
