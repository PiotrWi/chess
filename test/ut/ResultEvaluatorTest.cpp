#include <gtest/gtest.h>

#include <publicIf/Board.hpp>
#include <detail/MoveApplier.hpp>
#include <publicIf/NotationConversions.hpp>
#include <detail/ResultEvaluator.hpp>

#include <utils/BoardGenerationUtils.hpp>
#include <notations/coordinationnotation.hpp>

namespace
{

constexpr NOTATION::COLOR::color WHITE = NOTATION::COLOR::color::white;
constexpr NOTATION::COLOR::color BLACK = NOTATION::COLOR::color::black;

}  // namespace

class ResultEvaluatorShould
    : public ::testing::Test
{
public:
    void applyMoveAndStore(Board& board,
                           const char* moveStr,
                           const NOTATION::COLOR::color c)
    {
        auto move = notations::coordinates::createMove(moveStr, c);
        auto extendMove = convertMoveToExtended(board, move);
        MoveApplier::applyMove(board, move);
        sut.storeBoard(board, extendMove);
    }

    template<size_t N>
    void verifyChainOfMoves(Board& board,
                            const char*(&&moves)[N],
                            NOTATION::COLOR::color c,
                            Result finalResult)
    {
        for (auto i = 0u; i < N - 1; ++i)
        {
            applyMoveAndStore(board, moves[i], c++);
            ASSERT_EQ(sut.evaluate(), Result::ongoing);
        }
        applyMoveAndStore(board, moves[N-1], c);
        ASSERT_EQ(sut.evaluate(), finalResult);
    }

    ResultEvaluator sut;
};

TEST_F (ResultEvaluatorShould, findMate)
{
	Board board = utils::createBoard(utils::InitialBoardString);
    verifyChainOfMoves(board,
                       {"e2-e4", "e7-e5", "f1-c4", "b8-c6", "d1-h5", "g8-f6", "h5-f7"},
                       WHITE,
                       Result::whiteWon);
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

TEST_F (ResultEvaluatorShould, findMateOnWhite)
{
	Board board = utils::createBoard(
			"       ♜"
			"        "
			"        "
			"     ♚ ♔"
			"        "
			"        "
			"        "
			"        ", WHITE);

    sut.storeBoard(board, {});
    ASSERT_EQ(sut.evaluate(), Result::blackWon);
}

TEST_F (ResultEvaluatorShould, findDrawByNoMove)
{

    Board board = utils::createBoard(
			" ♚      "
			" ♙      "
			" ♔      "
			"        "
			"        "
			"        "
			"        "
			"        ", BLACK);

    sut.storeBoard(board, {});
    ASSERT_EQ(sut.evaluate(), Result::draw);
}

TEST_F (ResultEvaluatorShould, findDrawByRepeatitions)
{
    Board board = utils::createBoard(
                        "♜       "
            "♚       "
            "        "
            "        "
            "        "
            "        "
            "♔       "
            "♖       ", BLACK);

    verifyChainOfMoves(board,
                       {"a1-h1", "a8-h8", "h1-a1", "a8-h8", "a1-h1", "a8-h8", "h1-a1", "a8-h8"},
                       WHITE,
                       Result::draw);
}
