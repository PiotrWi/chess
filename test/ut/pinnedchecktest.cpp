#include <gtest/gtest.h>

#include "PositionAnalyzers/PinnedMovesChecker.hpp"
#include <utils/BoardGenerationUtils.hpp>
#include "core/NotationConversions.hpp"

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

    ASSERT_EQ(pinnes.verticallyPinned, 0ull);
    ASSERT_EQ(pinnes.horizontallyPinned, 0ull);
    ASSERT_EQ(pinnes.diagonallyPinnedFromLeftBottom, 0ull);
    ASSERT_EQ(pinnes.diagonallyPinnedFromLeftTop, 0ull);
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

    ASSERT_EQ(pinnes.verticallyPinned, "e4"_bit | "e6"_bit);
    ASSERT_EQ(pinnes.horizontallyPinned, "f5"_bit | "d5"_bit);
    ASSERT_EQ(pinnes.diagonallyPinnedFromLeftBottom, 0ull);
    ASSERT_EQ(pinnes.diagonallyPinnedFromLeftTop, 0ull);
    ASSERT_EQ(pinnes.allPinned, "e6"_bit | "f5"_bit | "d5"_bit | "e4"_bit);

    ASSERT_EQ(PinnedRegister["e6"_field], "e6"_bit | "e7"_bit | "e8"_bit);
    ASSERT_EQ(PinnedRegister["e4"_field], "e1"_bit | "e2"_bit | "e3"_bit | "e4"_bit);
    ASSERT_EQ(PinnedRegister["f5"_field], "f5"_bit | "g5"_bit | "h5"_bit);
    ASSERT_EQ(PinnedRegister["d5"_field], "a5"_bit | "b5"_bit | "c5"_bit | "d5"_bit);
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

    ASSERT_EQ(pinnes.verticallyPinned, 0ull);
    ASSERT_EQ(pinnes.horizontallyPinned, 0ull);
    ASSERT_EQ(pinnes.diagonallyPinnedFromLeftBottom, "f6"_bit | "d4"_bit);
    ASSERT_EQ(pinnes.diagonallyPinnedFromLeftTop, "f4"_bit | "d6"_bit );
    ASSERT_EQ(pinnes.allPinned, "d6"_bit | "f6"_bit | "d4"_bit | "f4"_bit);

    ASSERT_EQ(PinnedRegister["d6"_field], "d6"_bit | "c7"_bit);
    ASSERT_EQ(PinnedRegister["f6"_field], "f6"_bit | "g7"_bit | "h8"_bit);
    ASSERT_EQ(PinnedRegister["d4"_field], "b2"_bit | "c3"_bit | "d4"_bit);
    ASSERT_EQ(PinnedRegister["f4"_field], "f4"_bit | "g3"_bit);
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
    ASSERT_EQ(pinnes.verticallyPinned, "e6"_bit);
    ASSERT_EQ(pinnes.horizontallyPinned, "f5"_bit | "d5"_bit);
    ASSERT_EQ(pinnes.diagonallyPinnedFromLeftBottom, "f6"_bit | "d4"_bit);
    ASSERT_EQ(pinnes.diagonallyPinnedFromLeftTop, "f4"_bit | "d6"_bit );
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

    ASSERT_EQ(pinnes.verticallyPinned, "e6"_bit | "e4"_bit);
    ASSERT_EQ(pinnes.horizontallyPinned, "f5"_bit | "d5"_bit);
    ASSERT_EQ(pinnes.diagonallyPinnedFromLeftBottom, 0ull);
    ASSERT_EQ(pinnes.diagonallyPinnedFromLeftTop, 0ull);
    ASSERT_EQ(pinnes.allPinned, "e6"_bit | "f5"_bit | "d5"_bit | "e4"_bit);
}


TEST(PinnedCheckShall, shallRestrictBottomRightDiagonalPinToPinLine)
{
    auto board = utils::createBoard("        "
                                    "        "
                                    "        "
                                    "    ♔   "
                                    "        "
                                    "      ♗ "
                                    "       ♝"
                                    "        ");

    auto pinnes = findPinned(board, NOTATION::COLOR::color::white, "e5"_field);

    ASSERT_EQ(pinnes.diagonallyPinnedFromLeftTop, "g3"_bit);
    ASSERT_EQ(PinnedRegister["g3"_field], "f4"_bit | "g3"_bit | "h2"_bit);
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

    ASSERT_EQ(pinnes.verticallyPinned, 0ull);
    ASSERT_EQ(pinnes.horizontallyPinned, 0ull);
    ASSERT_EQ(pinnes.diagonallyPinnedFromLeftBottom, "d4"_bit | "f6"_bit);
    ASSERT_EQ(pinnes.diagonallyPinnedFromLeftTop, "d6"_bit | "f4"_bit);
    ASSERT_EQ(pinnes.allPinned, "d6"_bit | "f6"_bit | "d4"_bit | "f4"_bit);
}
