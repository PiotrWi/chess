#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <MoveApplier.hpp>

#include <utils/BoardGenerationUtils.hpp>
#include <utils/LongAlgebraicNotation.hpp>

namespace
{
    constexpr NOTATION::COLOR::color WHITE = NOTATION::COLOR::color::white;
    constexpr NOTATION::COLOR::color BLACK = NOTATION::COLOR::color::black;

    class ResultEvaluatorMock
    {
    public:
        MOCK_METHOD(void, storeBoard, (const Board&));
    };
}

using namespace ::testing;

TEST(MoveApplierShould, storeMoveToResultEvaluator)
{
    ResultEvaluatorMock resultEvaluatorMock;
    Board board = utils::createBoard(utils::InitialBoardString);

    Board expectedBoard = utils::createBoard(
            "♜♞♝♛♚♝♞♜"
            "♟♟♟♟♟♟♟♟"
            "        "
            "        "
            "    ♙   "
            "        "
            "♙♙♙♙ ♙♙♙"
            "♖♘♗♕♔♗♘♖", BLACK);
    utils::setMovedBit(expectedBoard, "e4");
    utils::setLastMove(expectedBoard, createMove("e2-e4", WHITE));

    EXPECT_CALL(resultEvaluatorMock, storeBoard(_));
    MoveApplier::applyMove(board, createMove("e2-e4", WHITE), resultEvaluatorMock);
    ASSERT_EQ(expectedBoard, board);
}
