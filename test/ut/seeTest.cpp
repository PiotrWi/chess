#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <detail/sse.hpp>
#include <notations/LongAlgebraicNotation.hpp>
#include <utils/BoardGenerationUtils.hpp>

namespace
{

constexpr NOTATION::COLOR::color WHITE = NOTATION::COLOR::color::white;
constexpr NOTATION::COLOR::color BLACK = NOTATION::COLOR::color::black;

}

TEST(seeTest, shallCalculateCorrectlyUndefendedPawn)
{
    auto board = utils::createBoard(
            "    ♚   "
            "♟       "
            "        "
            "        "
            "        "
            "        "
            "        "
            "♕   ♔   ", WHITE);
    auto move = notations::long_algebraic::createExtendedMove("a1xa7", WHITE, board);
    ASSERT_EQ(see(move, board, WHITE), PawnValue);
}

TEST(seeTest, shallCalculateCorrectlyUndefendedPawnForBlack)
{
    auto board = utils::createBoard(
            "   ♛♚   "
            "   ♙    "
            "        "
            "        "
            "        "
            "        "
            "        "
            "    ♔   ", BLACK);
    auto move = notations::long_algebraic::createExtendedMove("d8xd7", BLACK, board);
    ASSERT_EQ(see(move, board, BLACK), PawnValue);
}

TEST(seeTest, shallCalculateCorrectlyUndefendedRock)
{
    auto board = utils::createBoard(
            "    ♚   "
            "♜       "
            "        "
            "        "
            "        "
            "        "
            "        "
            "♕   ♔   ", WHITE);
    auto move = notations::long_algebraic::createExtendedMove("a1xa7", WHITE, board);
    ASSERT_EQ(see(move, board, WHITE), RockValue);
}

TEST(seeTest, shallCalculateCorrectlyUndefendedRockForBlack)
{
    auto board = utils::createBoard(
            "   ♛♚   "
            "   ♖    "
            "        "
            "        "
            "        "
            "        "
            "        "
            "    ♔   ", BLACK);
    auto move = notations::long_algebraic::createExtendedMove("d8xd7", BLACK, board);
    ASSERT_EQ(see(move, board, BLACK), RockValue);
}

TEST(seeTest, shallCalculateCorrectlyDefendedPawn)
{
    auto board = utils::createBoard(
        "    ♚   "
        "        "
        "♟       "
        " ♟      "
        "        "
        "  ♘     "
        "        "
        "    ♔   ", WHITE);

	auto move = notations::long_algebraic::createExtendedMove("Nc3xb5", WHITE, board);
    ASSERT_EQ(see(move, board, WHITE), PawnValue - KnightValue);
}

TEST(seeTest, shallCalculateCorrectlyDefendedPawnForBlack)
{
    auto board = utils::createBoard(
        "    ♚  ♝"
        "        "
        "        "
        "        "
        "        "
        "  ♙     "
        " ♙      "
        "    ♔   ", BLACK);

    auto move = notations::long_algebraic::createExtendedMove("Bh8xc3", BLACK, board);
    ASSERT_EQ(see(move, board, BLACK), PawnValue - BishopValue);
}

TEST(seeTest, shallGiveValueForPawnExchangeAhead)
{
    auto board = utils::createBoard(
        "    ♚   "
        "    ♟   "
        "   ♟    "
        "  ♙ ♙   "
        "        "
        "        "
        "        "
        "    ♔   ", WHITE);

    auto move = notations::long_algebraic::createExtendedMove("c5xd6", WHITE, board);
    ASSERT_EQ(see(move, board, WHITE), PawnValue);
}

TEST(seeTest, shallGiveValueForPawnExchangeEven)
{
    auto board = utils::createBoard(
        "    ♚   "
        "  ♟ ♟   "
        "   ♟    "
        "  ♙ ♙   "
        "        "
        "        "
        "        "
        "    ♔   ", WHITE);

    auto move = notations::long_algebraic::createExtendedMove("c5xd6", WHITE, board);
    ASSERT_EQ(see(move, board, WHITE), 0);
}

TEST(seeTest, shallGiveNoValueIfPawnDefendedByBlackKnight)
{
    auto board = utils::createBoard(
        "  ♞ ♚   "
        "        "
        "   ♟    "
        "  ♙     "
        "        "
        "        "
        "        "
        "    ♔   ", WHITE);

    auto move = notations::long_algebraic::createExtendedMove("c5xd6", WHITE, board);
    ASSERT_EQ(see(move, board, WHITE), 0);
}

TEST(seeTest, shallGivePawnWhen2KnightsAreOnAction)
{
    auto board = utils::createBoard(
        "  ♞ ♚   "
        "        "
        "   ♟    "
        "  ♙     "
        "    ♘   "
        "        "
        "        "
        "    ♔   ", WHITE);

    auto move = notations::long_algebraic::createExtendedMove("c5xd6", WHITE, board);
    ASSERT_EQ(see(move, board, WHITE), PawnValue);
}

TEST(seeTest, shallCorreactlyAnalyzePawnDefendedByBishop)
{
    auto board = utils::createBoard(
        "    ♚   "
        "        "
        " ♝      "
        "        "
        "   ♟    "
        "        "
        "     ♗  "
        "    ♔   ", WHITE);

    auto move = notations::long_algebraic::createExtendedMove("Bf2xd4", WHITE, board);
    ASSERT_EQ(see(move, board, WHITE), PawnValue - BishopValue);
}

TEST(seeTest, shallCorreactlyAnalyzePawnDefendedByBishopAndReatackedByBishop)
{
    auto board = utils::createBoard(
        "    ♚   "
        "        "
        " ♝      "
        "        "
        "   ♟    "
        "        "
        "     ♗  "
        "    ♔ ♗ ", WHITE);

    auto move = notations::long_algebraic::createExtendedMove("Bf2xd4", WHITE, board);
    ASSERT_EQ(see(move, board, WHITE), PawnValue);
}