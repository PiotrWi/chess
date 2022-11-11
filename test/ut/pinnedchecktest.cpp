#include <gtest/gtest.h>

#include <detail/PinnedMovesChecker.hpp>
#include <utils/BoardGenerationUtils.hpp>
#include <publicIf/NotationConversions.hpp>

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

    auto pinnes = findPinned(board, NOTATION::COLOR::color::white, "d1"_field);
    ASSERT_EQ(pinnes.allPinned, 0ull);
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

    auto pinnes = findPinned(board, NOTATION::COLOR::color::white, "e5"_field);
    ASSERT_EQ(pinnes.allPinned, "e6"_bit | "f5"_bit | "d5"_bit | "e4"_bit);
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

    auto pinnes = findPinned(board, NOTATION::COLOR::color::white, "e5"_field);
    ASSERT_EQ(pinnes.allPinned, "d6"_bit | "f6"_bit | "d4"_bit | "f4"_bit);
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

    auto pinnes = findPinned(board, NOTATION::COLOR::color::white, "e5"_field);
    ASSERT_EQ(pinnes.allPinned,
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

    auto pinnes = findPinned(board, NOTATION::COLOR::color::black, "e5"_field);
    ASSERT_EQ(pinnes.allPinned, "e6"_bit | "f5"_bit | "d5"_bit | "e4"_bit);
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
    auto pinnes = findPinned(board, NOTATION::COLOR::color::black, "e5"_field);
    ASSERT_EQ(pinnes.allPinned, "d6"_bit | "f6"_bit | "d4"_bit | "f4"_bit);
}
