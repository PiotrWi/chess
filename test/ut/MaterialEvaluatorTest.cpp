/*#include <gtest/gtest.h>
#include <utils/BoardGenerationUtils.hpp>
#include <common/evaluators/MaterialEvaluator.hpp>
#include <detail/MoveApplier.hpp>
#include <notations/coordinationnotation.hpp>

TEST(MaterialEvaluatorShall, EvaluateFirstPosition)
{
    auto initialBoard = utils::createBoard(utils::InitialBoardString,NOTATION::COLOR::color::white );

    ASSERT_EQ(0, materialEvaluator::evaluate(initialBoard, NOTATION::COLOR::color::white ));
    ASSERT_EQ(0, materialEvaluator::evaluate(initialBoard, NOTATION::COLOR::color::black ));
}

TEST(MaterialEvaluatorShall, EvaluateAfterFirstMoves)
{
    auto initialBoard = utils::createBoard(utils::InitialBoardString,NOTATION::COLOR::color::white);
    MoveApplier::applyMove(initialBoard,
                           notations::coordinates::createExtendedMove("e2-e4",
                                                                      NOTATION::COLOR::color::white,
                                                                      initialBoard));
    ASSERT_EQ(0, materialEvaluator::evaluate(initialBoard, NOTATION::COLOR::color::white));
    ASSERT_EQ(0, materialEvaluator::evaluate(initialBoard, NOTATION::COLOR::color::black));

    MoveApplier::applyMove(
            initialBoard,
            notations::coordinates::createExtendedMove("e7-e5",
                                                       NOTATION::COLOR::color::black,
                                                       initialBoard));
    ASSERT_EQ(0, materialEvaluator::evaluate(initialBoard, NOTATION::COLOR::color::white));
    ASSERT_EQ(0, materialEvaluator::evaluate(initialBoard, NOTATION::COLOR::color::black));
}


TEST(MaterialEvaluatorShall, EvaluateSinglePawn)
{
    const char* boardString =
        "♜♞♝♛♚♝♞♜"
        "♟♟♟♟♟♟♟♟"
        "        "
        "        "
        "        "
        "        "
        "♙♙♙ ♙♙♙♙"
        "♖♘♗♕♔♗♘♖";
    auto initialBoard = utils::createBoard(boardString,NOTATION::COLOR::color::white );
    ASSERT_EQ(-100, materialEvaluator::evaluate(initialBoard, NOTATION::COLOR::color::white ));
    ASSERT_EQ(100, materialEvaluator::evaluate(initialBoard, NOTATION::COLOR::color::black ));
}
*/