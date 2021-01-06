#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <iostream>

#include <detail/MoveApplier.hpp>
#include <publicIf/BoardIO.hpp>
#include <utils/BoardGenerationUtils.hpp>
#include <notations/LongAlgebraicNotation.hpp>

namespace
{
    constexpr NOTATION::COLOR::color WHITE = NOTATION::COLOR::color::white;
    constexpr NOTATION::COLOR::color BLACK = NOTATION::COLOR::color::black;

    class ResultEvaluatorMock
    {
    public:
        MOCK_METHOD(void, storeBoard, (const Board&));
        MOCK_METHOD(void, removeSingle, ());
    };

    uint64_t nopHash = 0;
}

using namespace ::testing;

class MoveApplierShould : public Test
{
public:
    ResultEvaluatorMock resultEvaluatorMock;
};

TEST_F(MoveApplierShould, storeMoveToResultEvaluator)
{
    Board board = utils::createBoard(utils::InitialBoardString);
//    Board beforeMove = board;

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
    utils::setValidEnPassant(expectedBoard, "e3");

    EXPECT_CALL(resultEvaluatorMock, storeBoard(_));
    MoveApplier::applyMove(board, nopHash, createMove("e2-e4", WHITE), resultEvaluatorMock);
    ASSERT_EQ(expectedBoard, board);
/*
    EXPECT_CALL(resultEvaluatorMock, removeSingle());
    MoveApplier::undoMove(board, memorial, resultEvaluatorMock);
    ASSERT_EQ(board, beforeMove);
    */
}

TEST_F(MoveApplierShould, doCastlesForWhite)
{
// expectations
    Board shortCastleExpectedBoard = utils::createBoard(
            "♜♞♝♛♚♝♞♜"
            "♟♟♟♟♟♟♟♟"
            "        "
            "        "
            "    ♙   "
            "        "
            "♙♙♙♙ ♙♙♙"
            "♖    ♖♔ ", BLACK);
    utils::setMovedBit(shortCastleExpectedBoard, "g1");
    utils::setMovedBit(shortCastleExpectedBoard, "f1");

    Board longCastleExpectedBoard = utils::createBoard(
            "♜♞♝♛♚♝♞♜"
            "♟♟♟♟♟♟♟♟"
            "        "
            "        "
            "    ♙   "
            "        "
            "♙♙♙♙ ♙♙♙"
            "  ♔♖   ♖", BLACK);
    utils::setMovedBit(longCastleExpectedBoard, "c1");
    utils::setMovedBit(longCastleExpectedBoard, "d1");

// preparation
    Board board = utils::createBoard("♜♞♝♛♚♝♞♜"
                                     "♟♟♟♟♟♟♟♟"
                                     "        "
                                     "        "
                                     "    ♙   "
                                     "        "
                                     "♙♙♙♙ ♙♙♙"
                                     "♖   ♔  ♖");

// short caste
    Board shortCaste = board;
    EXPECT_CALL(resultEvaluatorMock, storeBoard(_));
    MoveApplier::applyMove(shortCaste, nopHash, createMove("O-O", WHITE), resultEvaluatorMock);
    ASSERT_EQ(shortCastleExpectedBoard, shortCaste);

// long caste
    Board longCastle = board;
    EXPECT_CALL(resultEvaluatorMock, storeBoard(_));
    MoveApplier::applyMove(longCastle, nopHash, createMove("O-O-O", WHITE), resultEvaluatorMock);
    ASSERT_EQ(longCastleExpectedBoard, longCastle);
}


TEST_F(MoveApplierShould, doCastlesForBlack)
{
// expectations
    Board shortCastleExpectedBoard = utils::createBoard(
            "♜    ♜♚ "
            "♟♟♟♟♟♟♟♟"
            "        "
            "        "
            "    ♙   "
            "        "
            "♙♙♙♙ ♙♙♙"
            "♖   ♔  ♖", WHITE);
    utils::setMovedBit(shortCastleExpectedBoard, "g8");
    utils::setMovedBit(shortCastleExpectedBoard, "f8");

    Board longCastleExpectedBoard = utils::createBoard(
            "  ♚♜   ♜"
            "♟♟♟♟♟♟♟♟"
            "        "
            "        "
            "    ♙   "
            "        "
            "♙♙♙♙ ♙♙♙"
            "♖   ♔  ♖", WHITE);
    utils::setMovedBit(longCastleExpectedBoard, "c8");
    utils::setMovedBit(longCastleExpectedBoard, "d8");


// preparation
    Board board = utils::createBoard("♜   ♚  ♜"
                                     "♟♟♟♟♟♟♟♟"
                                     "        "
                                     "        "
                                     "    ♙   "
                                     "        "
                                     "♙♙♙♙ ♙♙♙"
                                     "♖   ♔  ♖", BLACK);

// short caste
    Board shortCaste = board;
    EXPECT_CALL(resultEvaluatorMock, storeBoard(_));
    MoveApplier::applyMove(shortCaste, nopHash, createMove("O-O", BLACK), resultEvaluatorMock);
    ASSERT_EQ(shortCastleExpectedBoard, shortCaste);

// long caste
    Board longCastle = board;
    EXPECT_CALL(resultEvaluatorMock, storeBoard(_));
    MoveApplier::applyMove(longCastle, nopHash, createMove("O-O-O", BLACK), resultEvaluatorMock);
    ASSERT_EQ(longCastleExpectedBoard, longCastle);
}

TEST_F(MoveApplierShould, apllyPromotionsWhite)
{
    Board promotedPawn = utils::createBoard(
            "  ♕ ♚   "
            "        "
            "        "
            "        "
            "        "
            "        "
            "        "
            "      ♔ ", BLACK);

    utils::setMovedBit(promotedPawn, "c8");

    Board board = utils::createBoard("    ♚   "
         "  ♙     "
         "        "
         "        "
         "        "
         "        "
         "        "
         "      ♔ ", WHITE);

    EXPECT_CALL(resultEvaluatorMock, storeBoard(_));
    MoveApplier::applyMove(board, nopHash, createMove("c7-c8=Q", WHITE), resultEvaluatorMock);
    ASSERT_EQ(promotedPawn, board);
}

TEST_F(MoveApplierShould, apllyPromotionsBlack)
{
    Board promotedPawn = utils::createBoard(
            "    ♚   "
            "        "
            "        "
            "        "
            "        "
            "        "
            "        "
            "♜     ♔ ", WHITE);

    utils::setMovedBit(promotedPawn, "a1");

    Board board = utils::createBoard("    ♚   "
                                     "        "
                                     "        "
                                     "        "
                                     "        "
                                     "        "
                                     "♟       "
                                     "      ♔ ", BLACK);

    EXPECT_CALL(resultEvaluatorMock, storeBoard(_));
    MoveApplier::applyMove(board, nopHash, createMove("a2-a1=R", WHITE), resultEvaluatorMock);
    ASSERT_EQ(promotedPawn, board);
}

TEST_F(MoveApplierShould, allowEnPassantForWhite)
{
    Board expectedBoard = utils::createBoard(
            "♜♞♝♛♚♝♞♜"
            "♟♟♟ ♟♟♟♟"
            "   ♙    "
            "        "
            "        "
            "        "
            "♙♙♙♙ ♙♙♙"
            "♖♘♗♕♔♗♘♖", BLACK);
    utils::setMovedBit(expectedBoard, "d6");

    Board board = utils::createBoard(
        "♜♞♝♛♚♝♞♜"
        "♟♟♟♟♟♟♟♟"
        "        "
        "    ♙   "
        "        "
        "        "
        "♙♙♙♙ ♙♙♙"
        "♖♘♗♕♔♗♘♖", BLACK);

    EXPECT_CALL(resultEvaluatorMock, storeBoard(_));
    MoveApplier::applyMove(board, nopHash, createMove("d7-d5", BLACK), resultEvaluatorMock);

    EXPECT_CALL(resultEvaluatorMock, storeBoard(_));
    MoveApplier::applyMove(board, nopHash, createMove("e5-d6", WHITE), resultEvaluatorMock);

    ASSERT_EQ(expectedBoard, board);
}

TEST_F(MoveApplierShould, allowEnPassantForBlack)
{
    Board expectedBoard = utils::createBoard(
            "♜♞♝♛♚♝♞♜"
            "♟♟♟ ♟♟♟♟"
            "        "
            "        "
            "        "
            "    ♟   "
            "♙♙♙♙ ♙♙♙"
            "♖♘♗♕♔♗♘♖", WHITE);
    utils::setMovedBit(expectedBoard, "e3");

    Board board = utils::createBoard(
            "♜♞♝♛♚♝♞♜"
            "♟♟♟ ♟♟♟♟"
            "        "
            "        "
            "   ♟    "
            "        "
            "♙♙♙♙♙♙♙♙"
            "♖♘♗♕♔♗♘♖", WHITE);

    EXPECT_CALL(resultEvaluatorMock, storeBoard(_));
    MoveApplier::applyMove(board, nopHash, createMove("e2-e4", WHITE), resultEvaluatorMock);
    utils::setValidEnPassant(board, "e3");
    EXPECT_CALL(resultEvaluatorMock, storeBoard(_));
    MoveApplier::applyMove(board, nopHash, createMove("d4-e3", BLACK), resultEvaluatorMock);

    ASSERT_EQ(expectedBoard, board);
}

