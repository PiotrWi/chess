#include <gtest/gtest.h>

#include <Board.hpp>
#include <CheckChecker.hpp>

#include <utils/BoardGenerationUtils.hpp>

/* PAWN based */
TEST(CheckCheckershould, FindCheckByPawn)
{
    Board board = utils::createBoard(
			"        "
			"   ♚    "
			"  ♙     "
			"        "
			"        "
			"        "
			"        "
			"    ♔   ");
	ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));
	ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));
}


TEST(CheckCheckershould, DoNotFindCheckByPawn)
{
    Board board = utils::createBoard(
			"        "
			"   ♚    "
			"   ♙    "
			"        "
			"        "
			"        "
			"        "
			"    ♔   ");
	ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));
	ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));
}

TEST(CheckCheckershould, DoFindCheckByPawn_2)
{
    Board board = utils::createBoard(
			"        "
			"   ♚♟   "
			"♙  ♙    "
			"       ♙"
			"        "
			"        "
			"   ♟    "
			"    ♔   ");
	ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));
	ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));
}

/* ROCK Based */
TEST(CheckCheckershould, FindCheckByRockHorizontaly)
{
    Board board = utils::createBoard(
			"        "
			"   ♚   ♖"
			"        "
			"        "
			"        "
			"       ♜"
			"        "
			"    ♔   ");
	ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));
	ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));
}

TEST(CheckCheckershould, FindCheckByRockHorizontaly_2)
{
    Board board = utils::createBoard(
			"        "
			"♖   ♚   "
			"        "
			"        "
			"        "
			"  ♜     "
			"        "
			"    ♔   ");
	ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));
	ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));
}

TEST(CheckCheckershould, FindCheckByRockHorizontaly_3)
{
    Board board = utils::createBoard(
			"        "
			"    ♚   "
			" ♖      "
			"        "
			"        "
			"        "
			"        "
			" ♜  ♔   ");
	ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));
	ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));
}

TEST(CheckCheckershould, FindCheckByRockVertically_1)
{
    Board board = utils::createBoard(
			"        "
			"    ♚   "
			" ♖      "
			"        "
			"        "
			"    ♜   "
			"        "
			"    ♔   ");
	ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));
	ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));
}

TEST(CheckCheckershould, FindCheckByRockVertically_2)
{
    Board board = utils::createBoard(
			"        "
			"       ♚"
			"       ♖"
			"        "
			"        "
			"     ♜  "
			"        "
			"    ♔   ");
	ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));
	ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));
}

TEST(CheckCheckershould, NotFindCheckByRockWhenPieceInBeetween)
{
    Board board = utils::createBoard(
			"        "
			"♖     ♝♚"
			"        "
			"        "
			"        "
			"    ♜   "
			"    ♝   "
			"    ♔   ");
	ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));
	ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));
}


TEST(CheckCheckershould, FindCheckByBishop)
{
    Board board = utils::createBoard(
			"        "
			"♖     ♝♚"
			"        "
			"        "
			"       ♝"
			"        "
			"        "
			"    ♔   ");
	ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));
	ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));
}

TEST(CheckCheckershould, FindCheckByBishop_2)
{
    Board board = utils::createBoard(
			"        "
			"♖     ♝♚"
			"        "
			"        "
			"        "
			"        "
			"        "
			" ♗   ♔  ");
	ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));
	ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));
}

TEST(CheckCheckershould, NotFindCheckByBishopWhenpieceBetween)
{
    Board board = utils::createBoard(
			"        "
			"      ♝♚"
			"        "
			"        "
			"    ♖   "
			"        "
			"        "
			" ♗   ♔  ");
	ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));
	ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));
}

TEST(CheckCheckershould, FindCheckByGueen)
{
    Board board = utils::createBoard(
			"        "
			"       ♚"
			"        "
			"        "
			"    ♕   "
			"        "
			"        "
			" ♛   ♔  ");
	ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));
	ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));
}

TEST(CheckCheckershould, FindCheckByKnight)
{
    Board board = utils::createBoard(
			"♜♞♝♛♚♝♞♜"
			"♟♟♟♟♟♟♟♟"
			"     ♘  "
			"        "
			"        "
			"  ♞     "
			"♙♙♙♙♙♙♙♙"
			"♖♘♗♔♕♗ ♖");
	ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));
	ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));
}

TEST(CheckCheckershould, FindCheckByKing)
{
    Board board = utils::createBoard(
			"        "
			"      ♔♚"
			"        "
			"        "
			"        "
			"        "
			"        "
			"        ");
	ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));
	ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));
}
