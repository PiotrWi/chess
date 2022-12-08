#include <gtest/gtest.h>

#include <publicIf/Board.hpp>
#include <utils/BoardGenerationUtils.hpp>
#include <detail/BitBoardsUtils.h>
#include <publicIf/NotationConversions.hpp>

namespace
{

uint64_t convertToUint64(const std::string& s)
{
    uint64_t out = {};
    for (unsigned int i = 0; i < 64; ++i)
    {
        if (s[i] == 'x')
        {
            auto row = 7 - (i / 8);
            auto col = i & 0b111;
            out |= 1ull << ((row << 3) | col);
        }
    }
    return out;
}

}

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
    ASSERT_EQ(getFullFrontSpan(board, NOTATION::COLOR::color::white), convertToUint64(expectedFrontSpan));
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
    ASSERT_EQ(getFullFrontSpan(board, NOTATION::COLOR::color::black), convertToUint64(expectedFrontSpan));
}