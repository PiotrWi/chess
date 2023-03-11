#include <gtest/gtest.h>

#include "core/Board.hpp"
#include "PositionAnalyzers/CheckChecker.hpp"
#include "core/NotationConversions.hpp"
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
    ASSERT_EQ(NotationConversions::getFieldNum("e1"), CheckChecker::findKing(board, NOTATION::COLOR::color::white));
    ASSERT_EQ(NotationConversions::getFieldNum("d7"), CheckChecker::findKing(board, NOTATION::COLOR::color::black));

    ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));
	ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));

    board = utils::createBoard(
            "        "
            "   ♚    "
            "    ♙   "
            "        "
            "        "
            "        "
            "        "
            "    ♔   ");

    ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));
    ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));

    board = utils::createBoard(
            "        "
            "   ♚    "
            "        "
            "        "
            "        "
            "        "
            "     ♟  "
            "    ♔   ");

    ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));
    ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));

    board = utils::createBoard(
            "        "
            "   ♚    "
            "        "
            "        "
            "        "
            "        "
            "   ♟    "
            "    ♔   ");

    ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));
    ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));
}

TEST(CheckCheckershould, DoNotFindCheckByPawn)
{
    Board board = utils::createBoard(
			"        "
			"♚       "
			"♙      ♙"
			"        "
			"        "
			"        "
			"        "
			"    ♔   ");
	ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));
	ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));

    board = utils::createBoard(
            "        "
            "   ♚    "
            "   ♙    "
            "        "
            "        "
            "        "
            "♟      ♟"
            "       ♔");
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

TEST(CheckCheckershould, FindCheckByQueen)
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

    board = utils::createBoard(
            "        "
            "        "
            "     ♘  "
            "        "
            "    ♚   "
            "        "
            "        "
            "   ♔    ");
    ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));

    board = utils::createBoard(
            "        "
            "        "
            "   ♘    "
            "        "
            "    ♚   "
            "        "
            "        "
            "   ♔    ");
    ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));

    board = utils::createBoard(
            "        "
            "        "
            "        "
            "  ♘     "
            "    ♚   "
            "        "
            "        "
            "   ♔    ");
    ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));

    board = utils::createBoard(
            "        "
            "        "
            "        "
            "      ♘ "
            "    ♚   "
            "        "
            "        "
            "   ♔    ");
    ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));

    board = utils::createBoard(
            "        "
            "        "
            "        "
            "        "
            "    ♚   "
            "      ♘ "
            "        "
            "   ♔    ");
    ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));


    board = utils::createBoard(
            "        "
            "        "
            "        "
            "        "
            "    ♚   "
            "  ♘     "
            "        "
            "   ♔    ");
    ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));
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

TEST(CheckCheckershould, workWithRapid)
{
    auto board = utils::createBoard(
            "        "
            "        "
            "        "
            "        "
            "        "
            "        "
            " ♟      "
            "♔      ♚");
    ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));
}

TEST(CheckCheckershould, Custom_1)
{
    auto board = utils::createBoard(
            "       ♚"
            "        "
            "        "
            "        "
            "       ♙"
            "      ♙♛"
            " ♟   ♙  "
            "     ♔  ");
    ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));
}

TEST(CheckCheckershould, Custom_2)
{
    auto board = utils::createBoard("    ♚   "
                                    "        "
                                    "        "
                                    "        "
                                    "        "
                                    "        "
                                    "     ♞♙♙"
                                    "      ♖♔");
    ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));
}

TEST(CheckCheckershould, Custom_3)
{
    Board board = utils::createBoard(
            "♜♞♝ ♚♝♞♜"
            "♟♟♟   ♟♟"
            "      ♙ "
            "   ♟♟♟  "
            "   ♙♙  ♖"
            "♘    ♘♔ "
            "♙♙♙  ♙♙ "
            "♖ ♗♕♗   ");
    ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));
}

TEST(CheckCheckershould, Custom_4)
{
    auto board = utils::createBoard("      ♚♛"
                                    "  ♛     "
                                    "   ♖♔♖  "
                                    "        "
                                    "   ♖ ♖  "
                                    "      ♛ "
                                    " ♛      "
                                    "        ");
    ASSERT_FALSE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));
}

TEST(CheckCheckershould, Custom_5)
{
    auto board = utils::createBoard("♜  ♘ ♜♚ "
                                    "♟ ♟♝ ♟♟ "
                                    "♕      ♟"
                                    "  ♟     "
                                    "   ♟    "
                                    " ♙  ♙   "
                                    "♙♗♙♙ ♙ ♙"
                                    "♖   ♔  ♛");
    ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::white));
}

TEST(CheckCheckershould, Custom_6)
{
    auto board = utils::createBoard(
            "♜  ♛♚♝ ♜"
            "♟♟♟  ♟♟ "
            "  ♗ ♝♞ ♟"
            "   ♟♟   "
            "       ♕"
            "  ♘ ♙   "
            "♙♙♙♙ ♙♙♙"
            "♖ ♗ ♔ ♘♖", NOTATION::COLOR::color::black);
    ASSERT_TRUE(CheckChecker::isCheckOn(board, NOTATION::COLOR::color::black));
}