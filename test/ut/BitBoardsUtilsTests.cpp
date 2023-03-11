#include <gtest/gtest.h>

#include "core/Board.hpp"
#include <utils/BoardGenerationUtils.hpp>
#include "core/bitboards/BitBoardsUtils.h"
#include "core/NotationConversions.hpp"


TEST(BitBoardsUtilsTests, shallCorrectlyCalculateFrontSpan)
{
    Board board = utils::createBoard(
            "        "
            "        "
            "        "
            "        "
            "        "
            "♙      ♙"
            "  ♙     "
            "    ♔   ");
    std::string expectedFrontSpan=
            "xxxx  xx"
            "xxxx  xx"
            "xxxx  xx"
            "xxxx  xx"
            "xxxx  xx"
            "xxxx   x"
            "  x     "
    "       ";
    ASSERT_EQ(getFullFrontSpan(board, NOTATION::COLOR::color::white), utils::convertToUint64(expectedFrontSpan));
}

TEST(BitBoardsUtilsTests, shallCorrectlyCalculateFrontSpanForBlack)
{
    Board board = utils::createBoard(
            "        "
            "♟   ♟  ♟"
            "        "
            "        "
            "        "
            "        "
            "        "
            "        ", NOTATION::COLOR::color::black);
    std::string expectedFrontSpan=
            "        "
            "x   x  x"
            "xx xxxxx"
            "xx xxxxx"
            "xx xxxxx"
            "xx xxxxx"
            "xx xxxxx"
            "xx xxxxx";
    ASSERT_EQ(getFullFrontSpan(board, NOTATION::COLOR::color::black), utils::convertToUint64(expectedFrontSpan));
}

TEST(BitBoardsUtilsTests, shallCorrectlyCalculatePawnsAttacksWhite)
{
    Board board = utils::createBoard(
            "        "
            "        "
            "        "
            "        "
            "        "
            "♙      ♙"
            "  ♙     "
            "    ♔   ");
    std::string expectedPawnsAttacks=
            "        "
            "        "
            "        "
            "        "
            " x    x "
            " x x    "
            "        "
            "        ";
    ASSERT_EQ(getAllFieldsAttackedByPawns(board, NOTATION::COLOR::color::white), utils::convertToUint64(expectedPawnsAttacks));
}

TEST(BitBoardsUtilsTests, shallCorrectlyCalculatePawnsAttacksBlack)
{
    Board board = utils::createBoard(
            "        "
            "♟   ♟  ♟"
            "        "
            "        "
            "        "
            "        "
            "        "
            "        ", NOTATION::COLOR::color::black);
    std::string expectedPawnsAttacks=
            "        "
            "        "
            " x x xx "
            "        "
            "        "
            "        "
            "        "
            "        ";
    ASSERT_EQ(getAllFieldsAttackedByPawns(board, NOTATION::COLOR::color::black), utils::convertToUint64(expectedPawnsAttacks));
}