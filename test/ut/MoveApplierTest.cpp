#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "MoveApplier/MoveApplier.hpp"
#include <hashing/zobrist.hpp>
#include "core/BoardIO.hpp"
#include <utils/BoardGenerationUtils.hpp>
#include <notations/coordinationnotation.hpp>
#include <notations/LongAlgebraicNotation.hpp>

namespace
{

constexpr NOTATION::COLOR::color WHITE = NOTATION::COLOR::color::white;
constexpr NOTATION::COLOR::color BLACK = NOTATION::COLOR::color::black;

class ResultEvaluatorMock
{
public:
    MOCK_METHOD(void, storeBoard, (const Board&));
    MOCK_METHOD(void, storeBoard, (const Board&, const ExtendedMove&));
    MOCK_METHOD(void, removeSingle, ());
};

uint64_t getHastForBoard(Board& board)
{
    return hash::hash(board);
}

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
    uint64_t hash = getHastForBoard(board);

    Board expectedBoard = utils::createBoard(
            "♜♞♝♛♚♝♞♜"
            "♟♟♟♟♟♟♟♟"
            "        "
            "        "
            "    ♙   "
            "        "
            "♙♙♙♙ ♙♙♙"
            "♖♘♗♕♔♗♘♖", BLACK);

    EXPECT_CALL(resultEvaluatorMock, storeBoard(_, _));
    MoveApplier::applyMove(board, hash, notations::long_algebraic::createExtendedMove("e2e4", WHITE, board), resultEvaluatorMock);

    ASSERT_EQ(expectedBoard, board);
    ASSERT_EQ(hash, getHastForBoard(expectedBoard));
}

TEST_F(MoveApplierShould, storeMoveToResultEvaluator2)
{
    Board board = utils::createBoard(utils::InitialBoardString);
    uint64_t hash = getHastForBoard(board);

    Board expectedBoard = utils::createBoard(
            "♜♞♝♛♚♝♞♜"
            "♟♟♟♟♟♟♟♟"
            "        "
            "        "
            "        "
            "    ♙   "
            "♙♙♙♙ ♙♙♙"
            "♖♘♗♕♔♗♘♖", BLACK);

    EXPECT_CALL(resultEvaluatorMock, storeBoard(_, _));
    MoveApplier::applyMove(board, hash, notations::long_algebraic::createExtendedMove("e2e3", WHITE, board), resultEvaluatorMock);

    ASSERT_EQ(expectedBoard, board);
    ASSERT_EQ(hash, getHastForBoard(expectedBoard));
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
    utils::revokeCastlingRights(shortCastleExpectedBoard, NOTATION::CASTLING_RIGHTS::WHITE_SHORT_BIT | NOTATION::CASTLING_RIGHTS::WHITE_LONG_BIT);

    Board longCastleExpectedBoard = utils::createBoard(
            "♜♞♝♛♚♝♞♜"
            "♟♟♟♟♟♟♟♟"
            "        "
            "        "
            "    ♙   "
            "        "
            "♙♙♙♙ ♙♙♙"
            "  ♔♖   ♖", BLACK);
    utils::revokeCastlingRights(longCastleExpectedBoard, NOTATION::CASTLING_RIGHTS::WHITE_SHORT_BIT | NOTATION::CASTLING_RIGHTS::WHITE_LONG_BIT);

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
    uint64_t hash = getHastForBoard(shortCaste);

    EXPECT_CALL(resultEvaluatorMock, storeBoard(_, _));
    MoveApplier::applyMove(shortCaste, hash, notations::long_algebraic::createExtendedMove("O-O", WHITE, shortCaste), resultEvaluatorMock);
    ASSERT_EQ(shortCastleExpectedBoard, shortCaste);
    ASSERT_EQ(hash, getHastForBoard(shortCastleExpectedBoard));

// long caste
    Board longCastle = board;
    hash = getHastForBoard(longCastle);

    EXPECT_CALL(resultEvaluatorMock, storeBoard(_, _));
    MoveApplier::applyMove(longCastle, hash, notations::long_algebraic::createExtendedMove("O-O-O", WHITE, longCastle), resultEvaluatorMock);
    ASSERT_EQ(longCastleExpectedBoard, longCastle);
    ASSERT_EQ(hash, getHastForBoard(longCastleExpectedBoard));
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
    utils::revokeCastlingRights(shortCastleExpectedBoard, NOTATION::CASTLING_RIGHTS::BLACK_SHORT_BIT | NOTATION::CASTLING_RIGHTS::BLACK_LONG_BIT);

    Board longCastleExpectedBoard = utils::createBoard(
            "  ♚♜   ♜"
            "♟♟♟♟♟♟♟♟"
            "        "
            "        "
            "    ♙   "
            "        "
            "♙♙♙♙ ♙♙♙"
            "♖   ♔  ♖", WHITE);
    utils::revokeCastlingRights(longCastleExpectedBoard, NOTATION::CASTLING_RIGHTS::BLACK_SHORT_BIT | NOTATION::CASTLING_RIGHTS::BLACK_LONG_BIT);

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
    uint64_t hash = getHastForBoard(shortCaste);

    EXPECT_CALL(resultEvaluatorMock, storeBoard(_, _));
    MoveApplier::applyMove(shortCaste, hash, notations::coordinates::createExtendedMove("O-O", BLACK, shortCaste), resultEvaluatorMock);
    ASSERT_EQ(shortCastleExpectedBoard, shortCaste);
    ASSERT_EQ(hash, hash::hash(shortCastleExpectedBoard));

// long caste
    Board longCastle = board;
    hash = getHastForBoard(longCastle);

    EXPECT_CALL(resultEvaluatorMock, storeBoard(_, _));
    MoveApplier::applyMove(longCastle, hash, notations::coordinates::createExtendedMove("O-O-O", BLACK, longCastle), resultEvaluatorMock);
    ASSERT_EQ(longCastleExpectedBoard, longCastle);
    ASSERT_EQ(hash, hash::hash(longCastleExpectedBoard));
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


    Board board = utils::createBoard("    ♚   "
         "  ♙     "
         "        "
         "        "
         "        "
         "        "
         "        "
         "      ♔ ", WHITE);
    auto hash = getHastForBoard(board);

    EXPECT_CALL(resultEvaluatorMock, storeBoard(_, _));
    MoveApplier::applyMove(board, hash, notations::coordinates::createExtendedMove("c7-c8=Q", WHITE, board), resultEvaluatorMock);
    ASSERT_EQ(promotedPawn, board);
    ASSERT_EQ(hash, hash::hash(promotedPawn));
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

    Board board = utils::createBoard("    ♚   "
                                     "        "
                                     "        "
                                     "        "
                                     "        "
                                     "        "
                                     "♟       "
                                     "      ♔ ", BLACK);
    auto hash = getHastForBoard(board);

    EXPECT_CALL(resultEvaluatorMock, storeBoard(_, _));
    MoveApplier::applyMove(board, hash, notations::coordinates::createExtendedMove("a2-a1=R", WHITE, board), resultEvaluatorMock);
    ASSERT_EQ(promotedPawn, board);
    ASSERT_EQ(hash, hash::hash(promotedPawn));
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

    Board intermeadieteBoard = utils::createBoard(
            "♜♞♝♛♚♝♞♜"
            "♟♟♟ ♟♟♟♟"
            "        "
            "   ♟♙   "
            "        "
            "        "
            "♙♙♙♙ ♙♙♙"
            "♖♘♗♕♔♗♘♖", WHITE);
    utils::setValidEnPassant(intermeadieteBoard, "d6");

    Board board = utils::createBoard(
        "♜♞♝♛♚♝♞♜"
        "♟♟♟♟♟♟♟♟"
        "        "
        "    ♙   "
        "        "
        "        "
        "♙♙♙♙ ♙♙♙"
        "♖♘♗♕♔♗♘♖", BLACK);
    auto hash = hash::hash(board);
    EXPECT_CALL(resultEvaluatorMock, storeBoard(_, _));
    MoveApplier::applyMove(board, hash, notations::coordinates::createExtendedMove("d7-d5", BLACK, board), resultEvaluatorMock);
    ASSERT_EQ(hash, hash::hash(intermeadieteBoard));

    EXPECT_CALL(resultEvaluatorMock, storeBoard(_, _));
    MoveApplier::applyMove(board, hash, notations::coordinates::createExtendedMove("e5-d6", WHITE, board), resultEvaluatorMock);

    ASSERT_EQ(expectedBoard, board);
    ASSERT_EQ(hash, hash::hash(expectedBoard));
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

    Board intermedieteBoard = utils::createBoard(
            "♜♞♝♛♚♝♞♜"
            "♟♟♟ ♟♟♟♟"
            "        "
            "        "
            "   ♟♙   "
            "        "
            "♙♙♙♙ ♙♙♙"
            "♖♘♗♕♔♗♘♖", BLACK);
    utils::setValidEnPassant(intermedieteBoard, "e3");

    Board board = utils::createBoard(
            "♜♞♝♛♚♝♞♜"
            "♟♟♟ ♟♟♟♟"
            "        "
            "        "
            "   ♟    "
            "        "
            "♙♙♙♙♙♙♙♙"
            "♖♘♗♕♔♗♘♖", WHITE);
    auto hash = hash::hash(board);

    EXPECT_CALL(resultEvaluatorMock, storeBoard(_, _));
    MoveApplier::applyMove(board, hash, notations::coordinates::createExtendedMove("e2-e4", WHITE, board), resultEvaluatorMock);
    utils::setValidEnPassant(board, "e3");
    EXPECT_CALL(resultEvaluatorMock, storeBoard(_, _));
    MoveApplier::applyMove(board, hash, notations::coordinates::createExtendedMove("d4-e3", BLACK, board), resultEvaluatorMock);

    ASSERT_EQ(expectedBoard, board);
}

// Problematic positions
TEST_F(MoveApplierShould, problematic_1)
{
    Board board = utils::createBoard(
        "♜♞♝.♚♝♞♜"
        "♟♟♟..♟♟♟"
        "....♟..."
        "...♟♙..."
        ".......♛"
        "........"
        "♙♙♙♙.♙♙♙"
        "♖♘♗♕♔♗♘♖");
    board.validEnPassant = 43;
    auto hash = hash::hash(board);

    auto move = notations::coordinates::createExtendedMove("e5-d6", WHITE, board);
    EXPECT_CALL(resultEvaluatorMock, storeBoard(_, _));

    MoveApplier::applyMove(board, hash, move, resultEvaluatorMock);
    ASSERT_EQ(hash, hash::hash(board));
}

TEST_F(MoveApplierShould, problematic_2)
{
    Board board = utils::createBoard("♜♞♝♛♚.♞♜"
        "♟♟♟♟♝♟♕♟"
        "....♟..."
        "........"
        "........"
        "....♙..."
        "♙♙♙♙.♙♙♙"
        "♖♘♗.♔♗♘♖", BLACK);

    auto hash = hash::hash(board);
    auto move = notations::coordinates::createExtendedMove("e7-b4", BLACK, board);
    EXPECT_CALL(resultEvaluatorMock, storeBoard(_, _));

    MoveApplier::applyMove(board, hash, move, resultEvaluatorMock);

    ASSERT_EQ(hash, hash::hash(board));
}

TEST_F(MoveApplierShould, problematic_3)
{
    Board board = utils::createBoard(
                "♜♞..♚♝.."
                "♟♟...♟♟."
                "..♟..♞.♜"
                "...♟♟.♘♟"
                ".♗♗....."
                ".♙♘.♙..."
                "♙.♝♙.♙♙♙"
                "♖...♔..♖"
                , WHITE);

    auto hash = hash::hash(board);
    auto move = notations::coordinates::createExtendedMove("c3-d5", WHITE, board);
    EXPECT_CALL(resultEvaluatorMock, storeBoard(_, _));

    MoveApplier::applyMove(board, hash, move, resultEvaluatorMock);

    Board expectedBoard = utils::createBoard(
                "♜♞..♚♝.."
                "♟♟...♟♟."
                "..♟..♞.♜"
                "...♘♟.♘♟"
                ".♗♗....."
                ".♙..♙..."
                "♙.♝♙.♙♙♙"
                "♖...♔..♖", BLACK);

    ASSERT_EQ(expectedBoard, board);
}
