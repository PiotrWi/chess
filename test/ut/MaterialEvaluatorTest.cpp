#include <gtest/gtest.h>
#include <utils/BoardGenerationUtils.hpp>
#include <detail/MoveApplier.hpp>
#include <notations/coordinationnotation.hpp>
#include <Common/MatherialEvaluator.hpp>

matherial_evaluator::VALUES_TYPE piecesValues(
        TPawnValue(100),
        TKnightValue(300),
        TBishopValue(300),
        TRockValue(500),
        TQueenValue(900));

TEST(MaterialEvaluatorShall, EvaluateFirstPosition)
{
    auto initialBoard = utils::createBoard(utils::InitialBoardString, NOTATION::COLOR::color::white );

    matherial_evaluator::RESULT_TYPE initialPositionPiecesDifference (
                TPawnCount{0},
                TBishopCount{0},
                TKnightCount{0},
                TRockCount{0},
                TQueenCount{0});
    ASSERT_EQ(initialPositionPiecesDifference, matherial_evaluator::get(initialBoard));
    ASSERT_EQ(0, matherial_evaluator::evaluate(initialBoard, NOTATION::COLOR::color::white, piecesValues));
    ASSERT_EQ(0, matherial_evaluator::evaluate(initialBoard, NOTATION::COLOR::color::black, piecesValues));
}

TEST(MaterialEvaluatorShall, EvaluateSinglePieceDifferencesBlackAhead)
{
    matherial_evaluator::RESULT_TYPE piecesDifference (
                TPawnCount{-1},
                TBishopCount{-1},
                TKnightCount{-1},
                TRockCount{-1},
                TQueenCount{-1});
    const char* boardString =
        "♜♞♝♛♚♝♞♜"
        "♟♟♟♟♟♟♟♟"
        "        "
        "        "
        "        "
        "        "
        "♙♙♙ ♙♙♙♙"
        "♖♘♗ ♔   ";
    auto board = utils::createBoard(boardString, NOTATION::COLOR::color::white );
    ASSERT_EQ(piecesDifference, matherial_evaluator::get(board));
    ASSERT_EQ(-2100, matherial_evaluator::evaluate(board, NOTATION::COLOR::color::white, piecesValues));
    ASSERT_EQ(2100, matherial_evaluator::evaluate(board, NOTATION::COLOR::color::black, piecesValues));
}

TEST(MaterialEvaluatorShall, EvaluateSinglePawnWhiteAhead)
{
    matherial_evaluator::RESULT_TYPE piecesDifference (
                TPawnCount{1},
                TBishopCount{1},
                TKnightCount{1},
                TRockCount{1},
                TQueenCount{1});
    const char* boardString =
        "♜♞♝ ♚   "
        "♟♟♟ ♟♟♟♟"
        "        "
        "        "
        "        "
        "        "
        "♙♙♙♙♙♙♙♙"
        "♖♘♗♕♔♗♘♖";
    auto board = utils::createBoard(boardString, NOTATION::COLOR::color::white );
    ASSERT_EQ(piecesDifference, matherial_evaluator::get(board));
    ASSERT_EQ(2100, matherial_evaluator::evaluate(board, NOTATION::COLOR::color::white, piecesValues));
    ASSERT_EQ(-2100, matherial_evaluator::evaluate(board, NOTATION::COLOR::color::black, piecesValues));
}

TEST(MaterialEvaluatorShall, EvaluateMultiplePieceDifferencesBlackAhead)
{
    matherial_evaluator::RESULT_TYPE piecesDifference (
                TPawnCount{-2},
                TBishopCount{-1},
                TKnightCount{-1},
                TRockCount{-1},
                TQueenCount{-1});
    const char* boardString =
        "♜♞♝♛♚♝♞♜"
        "♟♟♟♟♟♟♟ "
        "        "
        "        "
        "        "
        "        "
        "♙♙♙   ♙♙"
        "♖♘♗ ♔   ";
    auto board = utils::createBoard(boardString, NOTATION::COLOR::color::white );
    ASSERT_EQ(piecesDifference, matherial_evaluator::get(board));
    ASSERT_EQ(-2200, matherial_evaluator::evaluate(board, NOTATION::COLOR::color::white, piecesValues));
    ASSERT_EQ(2200, matherial_evaluator::evaluate(board, NOTATION::COLOR::color::black, piecesValues));
}
