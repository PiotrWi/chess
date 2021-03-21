#include <gtest/gtest.h>
#include <BoardEngine.hpp>

#include <publicIf/NotationConversions.hpp>
#include <notations/uci.hpp>
#include <utils/BoardGenerationUtils.hpp>
#include <hashing/zobrist.hpp>

TEST(WholeGame3, shallNoBrokeHashes)
{
    BoardEngine be = {};

    // 1
    auto em1 = ExtendedMove(12, 20, 4, 0, 2, 0);
    auto uci_em1 = notations::uci::createExtendedMove("e2e3",
                                                      NOTATION::COLOR::color::white, be.board);
    ASSERT_EQ(em1, uci_em1);
    auto expectedBoard_1 = utils::createBoard(
            "♜♞♝♛♚♝♞♜"
            "♟♟♟♟♟♟♟♟"
            "        "
            "        "
            "        "
            "    ♙   "
            "♙♙♙♙ ♙♙♙"
            "♖♘♗♕♔♗♘♖", NOTATION::COLOR::color::black);
    be.applyMove(uci_em1);
    ASSERT_EQ(be.board, expectedBoard_1);

    // 2
    auto em2 = ExtendedMove(52, 44, 4, 0, 3, 0);
    auto uci_em2 = notations::uci::createExtendedMove("e7e6",
                                                      NOTATION::COLOR::color::black, be.board);
    ASSERT_EQ(em2, uci_em2);
    auto expectedBoard_2 = utils::createBoard(
            "♜♞♝♛♚♝♞♜"
            "♟♟♟♟ ♟♟♟"
            "    ♟   "
            "        "
            "        "
            "    ♙   "
            "♙♙♙♙ ♙♙♙"
            "♖♘♗♕♔♗♘♖", NOTATION::COLOR::color::white);
    be.applyMove(uci_em2);
    ASSERT_EQ(be.board, expectedBoard_2);

    // 3
    auto em3 = ExtendedMove(3, 30, 0, 0, 10, 0);
    auto uci_em3 = notations::uci::createExtendedMove("d1g4",
                                                      NOTATION::COLOR::color::white, be.board);
    ASSERT_EQ(em3, uci_em3);
    auto expectedBoard_3 = utils::createBoard(
            "♜♞♝♛♚♝♞♜"
            "♟♟♟♟ ♟♟♟"
            "    ♟   "
            "        "
            "      ♕ "
            "    ♙   "
            "♙♙♙♙ ♙♙♙"
            "♖♘♗ ♔♗♘♖", NOTATION::COLOR::color::black);
    be.applyMove(uci_em3);
    ASSERT_EQ(be.board, expectedBoard_3);

    // 4
    auto em4 = ExtendedMove(61, 52, 0, 0, 9, 0);
    auto uci_em4 = notations::uci::createExtendedMove("f8e7",
                                                      NOTATION::COLOR::color::black, be.board);
    ASSERT_EQ(em4, uci_em4);
    auto expectedBoard_4 = utils::createBoard(
            "♜♞♝♛♚ ♞♜"
            "♟♟♟♟♝♟♟♟"
            "    ♟   "
            "        "
            "      ♕ "
            "    ♙   "
            "♙♙♙♙ ♙♙♙"
            "♖♘♗ ♔♗♘♖", NOTATION::COLOR::color::white);
    be.applyMove(uci_em4);
    ASSERT_EQ(be.board, expectedBoard_4);

    // 5
    auto em5 = ExtendedMove(30, 54, 2, 0, 10, 3);
    auto uci_em5 = notations::uci::createExtendedMove("g4g7",
                                                      NOTATION::COLOR::color::white, be.board);
    ASSERT_EQ(em5, uci_em5);
    auto expectedBoard_5 = utils::createBoard(
            "♜♞♝♛♚ ♞♜"
            "♟♟♟♟♝♟♕♟"
            "    ♟   "
            "        "
            "        "
            "    ♙   "
            "♙♙♙♙ ♙♙♙"
            "♖♘♗ ♔♗♘♖", NOTATION::COLOR::color::black);
    be.applyMove(uci_em5);
    ASSERT_EQ(be.board, expectedBoard_5);

    // 6
    auto em6 = ExtendedMove(52, 25, 0, 0, NOTATION::PIECES::BISHOP | NOTATION::COLOR::BLACK, 0);
    auto uci_em6 = notations::uci::createExtendedMove("e7b4",
                                                      NOTATION::COLOR::color::black, be.board);
    ASSERT_EQ(em6, uci_em6);
    auto expectedBoard_6 = utils::createBoard(
            "♜♞♝♛♚ ♞♜"
            "♟♟♟♟ ♟♕♟"
            "    ♟   "
            "        "
            " ♝      "
            "    ♙   "
            "♙♙♙♙ ♙♙♙"
            "♖♘♗ ♔♗♘♖", NOTATION::COLOR::color::white);
    be.applyMove(uci_em6);
    ASSERT_EQ(be.board, expectedBoard_6);
}