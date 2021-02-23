#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <detail/PinnedMovesChecker.hpp>
#include <utils/BoardGenerationUtils.hpp>
#include <publicIf/NotationConversions.hpp>

unsigned char operator "" _field(const char* fieldLiteral, size_t)
{
    return NotationConversions::getFieldNum(fieldLiteral);
}

uint64_t operator "" _bit(const char* fieldLiteral, size_t)
{
    return (1ull << NotationConversions::getFieldNum(fieldLiteral));
}

TEST(PinnedCheckShall, doNotFindPinWhileThereIsNo)
{
    Board board = utils::createBoard(
        "        "
        "      ♚ "
        "        "
        "        "
        "        "
        "        "
        "     ♟  "
        "   ♔    ");

    ASSERT_EQ(findPinned(board, NOTATION::COLOR::color::white, "d1"_field), 0ull);
}

TEST(PinnedCheckShall, shallFindRockPins)
{
    auto board = utils::createBoard("    ♜ ♚ "
                                    "        "
                                    "    ♖   "
                                    "♜  ♖♔♖ ♜"
                                    "    ♖   "
                                    "        "
                                    "        "
                                    "    ♜   ");

    ASSERT_EQ(findPinned(board, NOTATION::COLOR::color::white, "e5"_field), "e6"_bit | "f5"_bit | "d5"_bit | "e4"_bit);
}


TEST(PinnedCheckShall, shallFindBishopPins)
{
    auto board = utils::createBoard("      ♚♝"
                                    "  ♝     "
                                    "   ♖ ♖  "
                                    "    ♔   "
                                    "   ♖ ♖  "
                                    "      ♝ "
                                    " ♝      "
                                    "        ");

    ASSERT_EQ(findPinned(board, NOTATION::COLOR::color::white, "e5"_field), "d6"_bit | "f6"_bit | "d4"_bit | "f4"_bit);
}

TEST(PinnedCheckShall, shallFindQueensPins)
{
    auto board = utils::createBoard("      ♚♛"
                                    "  ♛ ♛   "
                                    "   ♖♖♖  "
                                    "♛♜ ♖♔♖ ♛"
                                    "   ♖♖♖  "
                                    "  ♛   ♛ "
                                    "    ♖   "
                                    "    ♛   ");

    ASSERT_EQ(findPinned(board, NOTATION::COLOR::color::white, "e5"_field),
              "d4"_bit | "f4"_bit | "d5"_bit | "f5"_bit | "d6"_bit | "e6"_bit | "f6"_bit);
}

TEST(PinnedCheckShall, shallFindRockPinsForBlack)
{
    auto board = utils::createBoard("    ♖ ♔ "
                                    "        "
                                    "    ♜   "
                                    "♖  ♜♚♜♖ "
                                    "    ♜   "
                                    "        "
                                    "        "
                                    "    ♖   ");

    ASSERT_EQ(findPinned(board, NOTATION::COLOR::color::black, "e5"_field), "e6"_bit | "f5"_bit | "d5"_bit | "e4"_bit);
}


TEST(PinnedCheckShall, shallFindBishopPinsForBlack)
{
    auto board = utils::createBoard("      ♔♗"
                                    "  ♗     "
                                    "   ♜ ♜  "
                                    "    ♚   "
                                    "   ♜ ♜  "
                                    "      ♗ "
                                    " ♗      "
                                    "        ");

    ASSERT_EQ(findPinned(board, NOTATION::COLOR::color::black, "e5"_field), "d6"_bit | "f6"_bit | "d4"_bit | "f4"_bit);
}
