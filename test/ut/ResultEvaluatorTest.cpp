#include <gtest/gtest.h>

#include <Board.hpp>
#include <BoardIO.hpp>

#include <ResultEvaluator.hpp>

#include <utils/BoardGenerationUtils.hpp>
#include <utils/LongAlgebraicNotation.hpp>

namespace
{

constexpr NOTATION::COLOR::color WHITE = NOTATION::COLOR::color::white;
constexpr NOTATION::COLOR::color BLACK = NOTATION::COLOR::color::black;

}

TEST (ResultEvaluatorShould, findMate)
{
	ResultEvaluator sut;

    Board board = utils::createBoard(utils::InitialBoardString);
    auto move = createMove("e2-e4", WHITE);
    applyMove(board, move);
    sut.storeBoard(board);
    std::cerr << board << std::endl;
    ASSERT_EQ(sut.evaluate(), Result::ongoing);

    move = createMove("e7-e5", BLACK);
    applyMove(board, move);
    sut.storeBoard(board);
    std::cerr << board << std::endl;
    ASSERT_EQ(sut.evaluate(), Result::ongoing);

    move = createMove("f1-c4", WHITE);
    applyMove(board, move);
    sut.storeBoard(board);
    std::cerr << board << std::endl;
    ASSERT_EQ(sut.evaluate(), Result::ongoing);

    move = createMove("b8-c6", BLACK);
    applyMove(board, move);
    sut.storeBoard(board);
    std::cerr << board << std::endl;
    ASSERT_EQ(sut.evaluate(), Result::ongoing);

    move = createMove("d1-h5", WHITE);
    applyMove(board, move);
    sut.storeBoard(board);
    std::cerr << board << std::endl;
    ASSERT_EQ(sut.evaluate(), Result::ongoing);

    move = createMove("g8-d6", BLACK);
    applyMove(board, move);
    sut.storeBoard(board);
    std::cerr << board << std::endl;
    ASSERT_EQ(sut.evaluate(), Result::ongoing);

    move = createMove("h5-f7", WHITE);
    applyMove(board, move);
    sut.storeBoard(board);
    std::cerr << board << std::endl;
    ASSERT_EQ(sut.evaluate(), Result::whiteWon);

/*
    "♜ ♝♛♚♝♞♜"
    "♟♟♟♟ ♟♟♟"
    "  ♞     "
    "    ♟  ♕"
    "  ♗ ♙   "
    "        "
    "♙♙♙♙ ♙♙♙"
    "♖♘♗ ♔ ♘♖";
*/

}
