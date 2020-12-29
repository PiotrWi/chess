#include <gtest/gtest.h>
#include <utils/BoardGenerationUtils.hpp>
#include <common/evaluators/MaterialEvaluator.hpp>
#include <detail/MoveApplier.hpp>
#include <notations/LongAlgebraicNotation.hpp>

TEST(MaterialEvaluatorShall, EvaluateFirstPosition)
{
    auto initialBoard = utils::createBoard(utils::InitialBoardString,NOTATION::COLOR::color::white );

    ASSERT_EQ(0, materialEvaluator::evaluate(initialBoard, NOTATION::COLOR::color::white ));
    ASSERT_EQ(0, materialEvaluator::evaluate(initialBoard, NOTATION::COLOR::color::black ));
}

TEST(MaterialEvaluatorShall, EvaluateAfterFirstMoves)
{
    /*const char* InitialBoardString =
        "♜♞♝♛♚♝♞♜"
        "♟♟♟♟♟♟♟♟"
        "        "
        "        "
        "        "
        "        "
        "♙♙♙♙♙♙♙♙"
        "♖♘♗♕♔♗♘♖";*/
    auto initialBoard = utils::createBoard(utils::InitialBoardString,NOTATION::COLOR::color::white );
    MoveApplier::applyMove(initialBoard, createMove("e2-e4", NOTATION::COLOR::color::white));
    ASSERT_EQ(0, materialEvaluator::evaluate(initialBoard, NOTATION::COLOR::color::white ));
    ASSERT_EQ(0, materialEvaluator::evaluate(initialBoard, NOTATION::COLOR::color::black ));

    MoveApplier::applyMove(initialBoard, createMove("e7-e5", NOTATION::COLOR::color::black));
    ASSERT_EQ(0, materialEvaluator::evaluate(initialBoard, NOTATION::COLOR::color::white ));
    ASSERT_EQ(0, materialEvaluator::evaluate(initialBoard, NOTATION::COLOR::color::black ));
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