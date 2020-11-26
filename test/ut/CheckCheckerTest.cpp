#include <gtest/gtest.h>

#include <cstring>
#include <utility>
#include <vector>

#include <Board.hpp>
#include <CheckChecker.hpp>

std::vector<std::pair<const char*, unsigned char>> symbolToPieceMapping = {
	std::make_pair("♜", NOTATION::COLOR::BLACK | NOTATION::PIECES::ROCK),
	std::make_pair("♞", NOTATION::COLOR::BLACK | NOTATION::PIECES::KNIGHT),
	std::make_pair("♝", NOTATION::COLOR::BLACK | NOTATION::PIECES::BISHOP),
	std::make_pair("♛", NOTATION::COLOR::BLACK | NOTATION::PIECES::QUEEN),
	std::make_pair("♚", NOTATION::COLOR::BLACK | NOTATION::PIECES::KING),
	std::make_pair("♟", NOTATION::COLOR::BLACK | NOTATION::PIECES::PAWN),
	std::make_pair("♙", NOTATION::COLOR::WHITE | NOTATION::PIECES::PAWN),
	std::make_pair("♕", NOTATION::COLOR::WHITE | NOTATION::PIECES::QUEEN),
	std::make_pair("♔", NOTATION::COLOR::WHITE | NOTATION::PIECES::KING),
	std::make_pair("♗", NOTATION::COLOR::WHITE | NOTATION::PIECES::BISHOP),
	std::make_pair("♘", NOTATION::COLOR::WHITE | NOTATION::PIECES::KNIGHT),
	std::make_pair("♖", NOTATION::COLOR::WHITE | NOTATION::PIECES::ROCK),
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
	for (const auto& map : symbolToPieceMapping)
	{
		bool match = true;
		for (auto i = 0u; i< bytesLen; ++i)
		{
			match &= map.first[i] == str[i];
		}
		if (match)
		{
			return map.second;
		}
	}
	return 0;
}

void initBoardByString(Board& board,
		const char* position,
		NOTATION::COLOR::color playerOnMove = NOTATION::COLOR::color::white,
		Move lastMove = {})
{
	for (int str_i = 0, i = 0; str_i < strlen(position); ++i)
	{
		auto len = unicodeLen(position[str_i]);
		if (len != 1)
		{
			auto piece = getCorrespondingFigure(&position[str_i], len);
			auto row = 7 - (i / 8);
			auto col = i & 0b111;

			board.fields[(row << 3) | col] = piece;
		}
		str_i += len;
	}
}

/* PAWN based */
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
	ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));
}


TEST(CheckCheckershould, DoNotFindCheckByPawn)
{
	Board board;
	auto position =
			"        "
			"   ♚    "
			"   ♙    "
			"        "
			"        "
			"        "
			"        "
			"    ♔   ";
	initBoardByString (board, position);
	ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));
	ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));
}

TEST(CheckCheckershould, DoFindCheckByPawn_2)
{
	Board board;
	auto position =
			"        "
			"   ♚♟   "
			"♙  ♙    "
			"       ♙"
			"        "
			"        "
			"   ♟    "
			"    ♔   ";
	initBoardByString (board, position);
	ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));
	ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));
}

/* ROCK Based */
TEST(CheckCheckershould, FindCheckByRockHorizontaly)
{
	Board board;
	auto position =
			"        "
			"   ♚   ♖"
			"        "
			"        "
			"        "
			"       ♜"
			"        "
			"    ♔   ";
	initBoardByString (board, position);
	ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));
	ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));
}

TEST(CheckCheckershould, FindCheckByRockHorizontaly_2)
{
	Board board;
	auto position =
			"        "
			"♖   ♚   "
			"        "
			"        "
			"        "
			"  ♜     "
			"        "
			"    ♔   ";
	initBoardByString (board, position);
	ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));
	ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));
}

TEST(CheckCheckershould, FindCheckByRockHorizontaly_3)
{
	Board board;
	auto position =
			"        "
			"    ♚   "
			" ♖      "
			"        "
			"        "
			"        "
			"        "
			" ♜  ♔   ";
	initBoardByString (board, position);
	ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));
	ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));
}

TEST(CheckCheckershould, FindCheckByRockVertically_1)
{
	Board board;
	auto position =
			"        "
			"    ♚   "
			" ♖      "
			"        "
			"        "
			"    ♜   "
			"        "
			"    ♔   ";
	initBoardByString (board, position);
	ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));
	ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));
}

TEST(CheckCheckershould, FindCheckByRockVertically_2)
{
	Board board;
	auto position =
			"        "
			"       ♚"
			"       ♖"
			"        "
			"        "
			"     ♜  "
			"        "
			"    ♔   ";
	initBoardByString (board, position);
	ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));
	ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));
}

TEST(CheckCheckershould, NotFindCheckByRockWhenPieceInBeetween)
{
	Board board;
	auto position =
			"        "
			"♖     ♝♚"
			"        "
			"        "
			"        "
			"    ♜   "
			"    ♝   "
			"    ♔   ";
	initBoardByString (board, position);
	ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));
	ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));
}
