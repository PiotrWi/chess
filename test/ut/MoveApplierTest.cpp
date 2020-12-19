#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <iostream>

#include <MoveApplier.hpp>
//#include <BoardIO.hpp>
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

class MoveApplierShould : public Test
{
public:
    ResultEvaluatorMock resultEvaluatorMock;
};

TEST_F(MoveApplierShould, storeMoveToResultEvaluator)
{
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
    utils::setLastMove(shortCastleExpectedBoard, createMove("e1-g1", WHITE));

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
    utils::setLastMove(longCastleExpectedBoard, createMove("O-O-O", WHITE));

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
    MoveApplier::applyMove(shortCaste, createMove("O-O", WHITE), resultEvaluatorMock);
    ASSERT_EQ(shortCastleExpectedBoard, shortCaste);

// long caste
    Board longCastle = board;
    EXPECT_CALL(resultEvaluatorMock, storeBoard(_));
    MoveApplier::applyMove(longCastle, createMove("O-O-O", WHITE), resultEvaluatorMock);
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
    utils::setLastMove(shortCastleExpectedBoard, createMove("e8-g8", BLACK));

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
    utils::setLastMove(longCastleExpectedBoard, createMove("O-O-O", BLACK));

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
    MoveApplier::applyMove(shortCaste, createMove("O-O", BLACK), resultEvaluatorMock);
    ASSERT_EQ(shortCastleExpectedBoard, shortCaste);

// long caste
    Board longCastle = board;
    EXPECT_CALL(resultEvaluatorMock, storeBoard(_));
    MoveApplier::applyMove(longCastle, createMove("O-O-O", BLACK), resultEvaluatorMock);
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
    utils::setLastMove(promotedPawn, createMove("c7-c8=Q", WHITE));

    Board board = utils::createBoard("    ♚   "
         "  ♙     "
         "        "
         "        "
         "        "
         "        "
         "        "
         "      ♔ ", WHITE);

    EXPECT_CALL(resultEvaluatorMock, storeBoard(_));
    MoveApplier::applyMove(board, createMove("c7-c8=Q", WHITE), resultEvaluatorMock);
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
    utils::setLastMove(promotedPawn, createMove("a2-a1=R", WHITE));

    Board board = utils::createBoard("    ♚   "
                                     "        "
                                     "        "
                                     "        "
                                     "        "
                                     "        "
                                     "♟       "
                                     "      ♔ ", BLACK);

    EXPECT_CALL(resultEvaluatorMock, storeBoard(_));
    MoveApplier::applyMove(board, createMove("a2-a1=R", WHITE), resultEvaluatorMock);
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
    utils::setLastMove(expectedBoard, createMove("e5-d6", WHITE));

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
    MoveApplier::applyMove(board, createMove("d7-d5", BLACK), resultEvaluatorMock);

    EXPECT_CALL(resultEvaluatorMock, storeBoard(_));
    MoveApplier::applyMove(board, createMove("e5-d6", WHITE), resultEvaluatorMock);

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
    utils::setLastMove(expectedBoard, createMove("d4-e3", BLACK));

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
    MoveApplier::applyMove(board, createMove("e2-e4", WHITE), resultEvaluatorMock);

    EXPECT_CALL(resultEvaluatorMock, storeBoard(_));
    MoveApplier::applyMove(board, createMove("d4-e3", BLACK), resultEvaluatorMock);

    ASSERT_EQ(expectedBoard, board);
}