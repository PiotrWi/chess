#include <gtest/gtest.h>
#include <BoardEngine.hpp>

#include "core/NotationConversions.hpp"
#include <notations/uci.hpp>
#include <utils/BoardGenerationUtils.hpp>
#include <hashing/zobrist.hpp>

TEST(WholeGame1, shallProperlyGenerateValidMoves)
{
    BoardEngine be = {};

    // PREPARATON
    const char* GIVEN_MOVES[] = {
            "e2e3",
            "e7e5",
            "d1h5",
            "b8c6",
            "b1c3",
            "g8f6",
            "h5g5",
            "h7h6",
            "g5h4",
            "d7d5",
            "f1b5",
            "c8e6",
            "b5c6",
            "b7c6",
            "h4a4",
            "e6d7",
            "g1f3",
            "f8d6",
            "b2b3",
            "e8g8",
            "c1b2",
            "c6c5",
            "a4a6",
            "d5d4",
            "c3b5",
            "e5e4",
            "b5d6",
            "e4f3",
            "d6b7",
            "f3g2",
            "b7d8",
            "g2h1q"
    };

// First move:
    be.applyMove(notations::uci::createExtendedMove(GIVEN_MOVES[0], be.board.playerOnMove, be.board));
    auto expectedBoard_1 = utils::createBoard(
            "♜♞♝♛♚♝♞♜"
            "♟♟♟♟♟♟♟♟"
            "        "
            "        "
            "        "
            "    ♙   "
            "♙♙♙♙ ♙♙♙"
            "♖♘♗♕♔♗♘♖", NOTATION::COLOR::color::black);
    ASSERT_EQ(be.board, expectedBoard_1);
    ASSERT_EQ(be.hash_, hash::hash(expectedBoard_1));

// Second move:
    be.applyMove(notations::uci::createExtendedMove(GIVEN_MOVES[1], be.board.playerOnMove, be.board));
    auto expectedBoard_2 = utils::createBoard(
            "♜♞♝♛♚♝♞♜"
            "♟♟♟♟ ♟♟♟"
            "        "
            "    ♟   "
            "        "
            "    ♙   "
            "♙♙♙♙ ♙♙♙"
            "♖♘♗♕♔♗♘♖", NOTATION::COLOR::color::white);
    ASSERT_EQ(be.board, expectedBoard_2);
    ASSERT_EQ(be.hash_, hash::hash(expectedBoard_2));

    // Third move:
    be.applyMove(notations::uci::createExtendedMove(GIVEN_MOVES[2], be.board.playerOnMove, be.board));
    auto expectedBoard_3 = utils::createBoard(
            "♜♞♝♛♚♝♞♜"
            "♟♟♟♟ ♟♟♟"
            "        "
            "    ♟  ♕"
            "        "
            "    ♙   "
            "♙♙♙♙ ♙♙♙"
            "♖♘♗ ♔♗♘♖", NOTATION::COLOR::color::black);
    ASSERT_EQ(be.board, expectedBoard_3);
    ASSERT_EQ(be.hash_, hash::hash(expectedBoard_3));

    // Fourth move:
    be.applyMove(notations::uci::createExtendedMove(GIVEN_MOVES[3], be.board.playerOnMove, be.board));
    auto expectedBoard_4 = utils::createBoard(
            "♜ ♝♛♚♝♞♜"
            "♟♟♟♟ ♟♟♟"
            "  ♞     "
            "    ♟  ♕"
            "        "
            "    ♙   "
            "♙♙♙♙ ♙♙♙"
            "♖♘♗ ♔♗♘♖", NOTATION::COLOR::color::white);
    ASSERT_EQ(be.board, expectedBoard_4);
    ASSERT_EQ(be.hash_, hash::hash(expectedBoard_4));

    // Fifth move:
    be.applyMove(notations::uci::createExtendedMove(GIVEN_MOVES[4], be.board.playerOnMove, be.board));
    auto expectedBoard_5 = utils::createBoard(
            "♜ ♝♛♚♝♞♜"
            "♟♟♟♟ ♟♟♟"
            "  ♞     "
            "    ♟  ♕"
            "        "
            "  ♘ ♙   "
            "♙♙♙♙ ♙♙♙"
            "♖ ♗ ♔♗♘♖", NOTATION::COLOR::color::black);
    ASSERT_EQ(be.board, expectedBoard_5);
    ASSERT_EQ(be.hash_, hash::hash(expectedBoard_5));

    // Sixth move:
    be.applyMove(notations::uci::createExtendedMove(GIVEN_MOVES[5], be.board.playerOnMove, be.board));
    auto expectedBoard_6 = utils::createBoard(
            "♜ ♝♛♚♝ ♜"
            "♟♟♟♟ ♟♟♟"
            "  ♞  ♞  "
            "    ♟  ♕"
            "        "
            "  ♘ ♙   "
            "♙♙♙♙ ♙♙♙"
            "♖ ♗ ♔♗♘♖", NOTATION::COLOR::color::white);
    ASSERT_EQ(be.board, expectedBoard_6);
    ASSERT_EQ(be.hash_, hash::hash(expectedBoard_6));

    // Seventh move:
    be.applyMove(notations::uci::createExtendedMove(GIVEN_MOVES[6], be.board.playerOnMove, be.board));
    auto expectedBoard_7 = utils::createBoard(
            "♜ ♝♛♚♝ ♜"
            "♟♟♟♟ ♟♟♟"
            "  ♞  ♞  "
            "    ♟ ♕ "
            "        "
            "  ♘ ♙   "
            "♙♙♙♙ ♙♙♙"
            "♖ ♗ ♔♗♘♖", NOTATION::COLOR::color::black);
    ASSERT_EQ(be.board, expectedBoard_7);
    ASSERT_EQ(be.hash_, hash::hash(expectedBoard_7));

    // Eight move:
    be.applyMove(notations::uci::createExtendedMove(GIVEN_MOVES[7], be.board.playerOnMove, be.board));
    auto expectedBoard_8 = utils::createBoard(
            "♜ ♝♛♚♝ ♜"
            "♟♟♟♟ ♟♟ "
            "  ♞  ♞ ♟"
            "    ♟ ♕ "
            "        "
            "  ♘ ♙   "
            "♙♙♙♙ ♙♙♙"
            "♖ ♗ ♔♗♘♖", NOTATION::COLOR::color::white);
    ASSERT_EQ(be.board, expectedBoard_8);
    ASSERT_EQ(be.hash_, hash::hash(expectedBoard_8));

    // Ninth
    be.applyMove(notations::uci::createExtendedMove(GIVEN_MOVES[8], be.board.playerOnMove, be.board));
    auto expectedBoard_9 = utils::createBoard(
            "♜ ♝♛♚♝ ♜"
            "♟♟♟♟ ♟♟ "
            "  ♞  ♞ ♟"
            "    ♟   "
            "       ♕"
            "  ♘ ♙   "
            "♙♙♙♙ ♙♙♙"
            "♖ ♗ ♔♗♘♖", NOTATION::COLOR::color::black);
    ASSERT_EQ(be.board, expectedBoard_9);
    ASSERT_EQ(be.hash_, hash::hash(expectedBoard_9));

    // ten
    be.applyMove(notations::uci::createExtendedMove(GIVEN_MOVES[9], be.board.playerOnMove, be.board));
    auto expectedBoard_10 = utils::createBoard(
            "♜ ♝♛♚♝ ♜"
            "♟♟♟  ♟♟ "
            "  ♞  ♞ ♟"
            "   ♟♟   "
            "       ♕"
            "  ♘ ♙   "
            "♙♙♙♙ ♙♙♙"
            "♖ ♗ ♔♗♘♖", NOTATION::COLOR::color::white);
    ASSERT_EQ(be.board, expectedBoard_10);
    ASSERT_EQ(be.hash_, hash::hash(expectedBoard_10));

    // 11
    be.applyMove(notations::uci::createExtendedMove(GIVEN_MOVES[10], be.board.playerOnMove, be.board));
    auto expectedBoard_11 = utils::createBoard(
            "♜ ♝♛♚♝ ♜"
            "♟♟♟  ♟♟ "
            "  ♞  ♞ ♟"
            " ♗ ♟♟   "
            "       ♕"
            "  ♘ ♙   "
            "♙♙♙♙ ♙♙♙"
            "♖ ♗ ♔ ♘♖", NOTATION::COLOR::color::black);
    ASSERT_EQ(be.board, expectedBoard_11);
    ASSERT_EQ(be.hash_, hash::hash(expectedBoard_11));

    // 12
    be.applyMove(notations::uci::createExtendedMove(GIVEN_MOVES[11], be.board.playerOnMove, be.board));
    auto expectedBoard_12 = utils::createBoard(
            "♜  ♛♚♝ ♜"
            "♟♟♟  ♟♟ "
            "  ♞ ♝♞ ♟"
            " ♗ ♟♟   "
            "       ♕"
            "  ♘ ♙   "
            "♙♙♙♙ ♙♙♙"
            "♖ ♗ ♔ ♘♖", NOTATION::COLOR::color::white);
    ASSERT_EQ(be.board, expectedBoard_12);
    ASSERT_EQ(be.hash_, hash::hash(expectedBoard_12));

    // 13
    be.applyMove(notations::uci::createExtendedMove(GIVEN_MOVES[12], be.board.playerOnMove, be.board));
    auto expectedBoard_13 = utils::createBoard(
            "♜  ♛♚♝ ♜"
            "♟♟♟  ♟♟ "
            "  ♗ ♝♞ ♟"
            "   ♟♟   "
            "       ♕"
            "  ♘ ♙   "
            "♙♙♙♙ ♙♙♙"
            "♖ ♗ ♔ ♘♖", NOTATION::COLOR::color::black);
    ASSERT_EQ(be.board, expectedBoard_13);
    ASSERT_EQ(be.hash_, hash::hash(expectedBoard_13));

    // 14
    be.applyMove(notations::uci::createExtendedMove(GIVEN_MOVES[13], be.board.playerOnMove, be.board));
    auto expectedBoard_14 = utils::createBoard(
            "♜  ♛♚♝ ♜"
            "♟ ♟  ♟♟ "
            "  ♟ ♝♞ ♟"
            "   ♟♟   "
            "       ♕"
            "  ♘ ♙   "
            "♙♙♙♙ ♙♙♙"
            "♖ ♗ ♔ ♘♖", NOTATION::COLOR::color::white);
    ASSERT_EQ(be.board, expectedBoard_14);
    ASSERT_EQ(be.hash_, hash::hash(expectedBoard_14));

    // 15
    be.applyMove(notations::uci::createExtendedMove(GIVEN_MOVES[14], be.board.playerOnMove, be.board));
    auto expectedBoard_15 = utils::createBoard(
            "♜  ♛♚♝ ♜"
            "♟ ♟  ♟♟ "
            "  ♟ ♝♞ ♟"
            "   ♟♟   "
            "♕       "
            "  ♘ ♙   "
            "♙♙♙♙ ♙♙♙"
            "♖ ♗ ♔ ♘♖", NOTATION::COLOR::color::black);
    ASSERT_EQ(be.board, expectedBoard_15);
    ASSERT_EQ(be.hash_, hash::hash(expectedBoard_15));

    // 16
    be.applyMove(notations::uci::createExtendedMove(GIVEN_MOVES[15], be.board.playerOnMove, be.board));
    auto expectedBoard_16 = utils::createBoard(
            "♜  ♛♚♝ ♜"
            "♟ ♟♝ ♟♟ "
            "  ♟  ♞ ♟"
            "   ♟♟   "
            "♕       "
            "  ♘ ♙   "
            "♙♙♙♙ ♙♙♙"
            "♖ ♗ ♔ ♘♖", NOTATION::COLOR::color::white);
    ASSERT_EQ(be.board, expectedBoard_16);
    ASSERT_EQ(be.hash_, hash::hash(expectedBoard_16));

    // 17
    be.applyMove(notations::uci::createExtendedMove(GIVEN_MOVES[16], be.board.playerOnMove, be.board));
    auto expectedBoard_17 = utils::createBoard(
            "♜  ♛♚♝ ♜"
            "♟ ♟♝ ♟♟ "
            "  ♟  ♞ ♟"
            "   ♟♟   "
            "♕       "
            "  ♘ ♙♘  "
            "♙♙♙♙ ♙♙♙"
            "♖ ♗ ♔  ♖", NOTATION::COLOR::color::black);
    ASSERT_EQ(be.board, expectedBoard_17);
    ASSERT_EQ(be.hash_, hash::hash(expectedBoard_17));

    // 18
    be.applyMove(notations::uci::createExtendedMove(GIVEN_MOVES[17], be.board.playerOnMove, be.board));
    auto expectedBoard_18 = utils::createBoard(
            "♜  ♛♚  ♜"
            "♟ ♟♝ ♟♟ "
            "  ♟♝ ♞ ♟"
            "   ♟♟   "
            "♕       "
            "  ♘ ♙♘  "
            "♙♙♙♙ ♙♙♙"
            "♖ ♗ ♔  ♖", NOTATION::COLOR::color::white);
    ASSERT_EQ(be.board, expectedBoard_18);
    ASSERT_EQ(be.hash_, hash::hash(expectedBoard_18));

    // 19
    be.applyMove(notations::uci::createExtendedMove(GIVEN_MOVES[18], be.board.playerOnMove, be.board));
    auto expectedBoard_19 = utils::createBoard(
            "♜  ♛♚  ♜"
            "♟ ♟♝ ♟♟ "
            "  ♟♝ ♞ ♟"
            "   ♟♟   "
            "♕       "
            " ♙♘ ♙♘  "
            "♙ ♙♙ ♙♙♙"
            "♖ ♗ ♔  ♖", NOTATION::COLOR::color::black);
    ASSERT_EQ(be.board, expectedBoard_19);
    ASSERT_EQ(be.hash_, hash::hash(expectedBoard_19));

    // 20
    be.applyMove(notations::uci::createExtendedMove(GIVEN_MOVES[19], be.board.playerOnMove, be.board));
    auto expectedBoard_20 = utils::createBoard(
            "♜  ♛ ♜♚ "
            "♟ ♟♝ ♟♟ "
            "  ♟♝ ♞ ♟"
            "   ♟♟   "
            "♕       "
            " ♙♘ ♙♘  "
            "♙ ♙♙ ♙♙♙"
            "♖ ♗ ♔  ♖", NOTATION::COLOR::color::white);
    utils::revokeCastlingRights(expectedBoard_20, NOTATION::CASTLING_RIGHTS::BLACK_LONG_BIT);
    utils::revokeCastlingRights(expectedBoard_20, NOTATION::CASTLING_RIGHTS::BLACK_SHORT_BIT);
    ASSERT_EQ(be.board, expectedBoard_20);
    ASSERT_EQ(be.hash_, hash::hash(expectedBoard_20));

    // 21
    be.applyMove(notations::uci::createExtendedMove(GIVEN_MOVES[20], be.board.playerOnMove, be.board));
    auto expectedBoard_21 = utils::createBoard(
            "♜  ♛ ♜♚ "
            "♟ ♟♝ ♟♟ "
            "  ♟♝ ♞ ♟"
            "   ♟♟   "
            "♕       "
            " ♙♘ ♙♘  "
            "♙♗♙♙ ♙♙♙"
            "♖   ♔  ♖", NOTATION::COLOR::color::black);
    utils::revokeCastlingRights(expectedBoard_21, NOTATION::CASTLING_RIGHTS::BLACK_LONG_BIT);
    utils::revokeCastlingRights(expectedBoard_21, NOTATION::CASTLING_RIGHTS::BLACK_SHORT_BIT);
    ASSERT_EQ(be.board, expectedBoard_21);
    ASSERT_EQ(be.hash_, hash::hash(expectedBoard_21));

    // 22
    be.applyMove(notations::uci::createExtendedMove(GIVEN_MOVES[21], be.board.playerOnMove, be.board));
    auto expectedBoard_22 = utils::createBoard(
            "♜  ♛ ♜♚ "
            "♟ ♟♝ ♟♟ "
            "   ♝ ♞ ♟"
            "  ♟♟♟   "
            "♕       "
            " ♙♘ ♙♘  "
            "♙♗♙♙ ♙♙♙"
            "♖   ♔  ♖", NOTATION::COLOR::color::white);
    utils::revokeCastlingRights(expectedBoard_22, NOTATION::CASTLING_RIGHTS::BLACK_LONG_BIT);
    utils::revokeCastlingRights(expectedBoard_22, NOTATION::CASTLING_RIGHTS::BLACK_SHORT_BIT);
    ASSERT_EQ(be.board, expectedBoard_22);
    ASSERT_EQ(be.hash_, hash::hash(expectedBoard_22));

    // 23
    be.applyMove(notations::uci::createExtendedMove(GIVEN_MOVES[22], be.board.playerOnMove, be.board));
    auto expectedBoard_23 = utils::createBoard(
            "♜  ♛ ♜♚ "
            "♟ ♟♝ ♟♟ "
            "♕  ♝ ♞ ♟"
            "  ♟♟♟   "
            "        "
            " ♙♘ ♙♘  "
            "♙♗♙♙ ♙♙♙"
            "♖   ♔  ♖", NOTATION::COLOR::color::black);
    utils::revokeCastlingRights(expectedBoard_23, NOTATION::CASTLING_RIGHTS::BLACK_LONG_BIT);
    utils::revokeCastlingRights(expectedBoard_23, NOTATION::CASTLING_RIGHTS::BLACK_SHORT_BIT);
    ASSERT_EQ(be.board, expectedBoard_23);
    ASSERT_EQ(be.hash_, hash::hash(expectedBoard_23));

    // 24
    be.applyMove(notations::uci::createExtendedMove(GIVEN_MOVES[23], be.board.playerOnMove, be.board));
    auto expectedBoard_24 = utils::createBoard(
            "♜  ♛ ♜♚ "
            "♟ ♟♝ ♟♟ "
            "♕  ♝ ♞ ♟"
            "  ♟ ♟   "
            "   ♟    "
            " ♙♘ ♙♘  "
            "♙♗♙♙ ♙♙♙"
            "♖   ♔  ♖", NOTATION::COLOR::color::white);
    utils::revokeCastlingRights(expectedBoard_24, NOTATION::CASTLING_RIGHTS::BLACK_LONG_BIT);
    utils::revokeCastlingRights(expectedBoard_24, NOTATION::CASTLING_RIGHTS::BLACK_SHORT_BIT);
    ASSERT_EQ(be.board, expectedBoard_24);
    ASSERT_EQ(be.hash_, hash::hash(expectedBoard_24));

    // 25
    be.applyMove(notations::uci::createExtendedMove(GIVEN_MOVES[24], be.board.playerOnMove, be.board));
    auto expectedBoard_25 = utils::createBoard(
            "♜  ♛ ♜♚ "
            "♟ ♟♝ ♟♟ "
            "♕  ♝ ♞ ♟"
            " ♘♟ ♟   "
            "   ♟    "
            " ♙  ♙♘  "
            "♙♗♙♙ ♙♙♙"
            "♖   ♔  ♖", NOTATION::COLOR::color::black);
    utils::revokeCastlingRights(expectedBoard_25, NOTATION::CASTLING_RIGHTS::BLACK_LONG_BIT);
    utils::revokeCastlingRights(expectedBoard_25, NOTATION::CASTLING_RIGHTS::BLACK_SHORT_BIT);
    ASSERT_EQ(be.board, expectedBoard_25);
    ASSERT_EQ(be.hash_, hash::hash(expectedBoard_25));

    // 26
    be.applyMove(notations::uci::createExtendedMove(GIVEN_MOVES[25], be.board.playerOnMove, be.board));
    auto expectedBoard_26 = utils::createBoard(
            "♜  ♛ ♜♚ "
            "♟ ♟♝ ♟♟ "
            "♕  ♝ ♞ ♟"
            " ♘♟     "
            "   ♟♟   "
            " ♙  ♙♘  "
            "♙♗♙♙ ♙♙♙"
            "♖   ♔  ♖", NOTATION::COLOR::color::white);
    utils::revokeCastlingRights(expectedBoard_26, NOTATION::CASTLING_RIGHTS::BLACK_LONG_BIT);
    utils::revokeCastlingRights(expectedBoard_26, NOTATION::CASTLING_RIGHTS::BLACK_SHORT_BIT);
    ASSERT_EQ(be.board, expectedBoard_26);
    ASSERT_EQ(be.hash_, hash::hash(expectedBoard_26));

    // 27
    be.applyMove(notations::uci::createExtendedMove(GIVEN_MOVES[26], be.board.playerOnMove, be.board));
    auto expectedBoard_27 = utils::createBoard(
            "♜  ♛ ♜♚ "
            "♟ ♟♝ ♟♟ "
            "♕  ♘ ♞ ♟"
            "  ♟     "
            "   ♟♟   "
            " ♙  ♙♘  "
            "♙♗♙♙ ♙♙♙"
            "♖   ♔  ♖", NOTATION::COLOR::color::black);
    utils::revokeCastlingRights(expectedBoard_27, NOTATION::CASTLING_RIGHTS::BLACK_LONG_BIT);
    utils::revokeCastlingRights(expectedBoard_27, NOTATION::CASTLING_RIGHTS::BLACK_SHORT_BIT);
    ASSERT_EQ(be.board, expectedBoard_27);
    ASSERT_EQ(be.hash_, hash::hash(expectedBoard_27));

    // 28
    be.applyMove(notations::uci::createExtendedMove(GIVEN_MOVES[27], be.board.playerOnMove, be.board));
    auto expectedBoard_28 = utils::createBoard(
            "♜  ♛ ♜♚ "
            "♟ ♟♝ ♟♟ "
            "♕  ♘ ♞ ♟"
            "  ♟     "
            "   ♟    "
            " ♙  ♙♟  "
            "♙♗♙♙ ♙♙♙"
            "♖   ♔  ♖", NOTATION::COLOR::color::white);
    utils::revokeCastlingRights(expectedBoard_28, NOTATION::CASTLING_RIGHTS::BLACK_LONG_BIT);
    utils::revokeCastlingRights(expectedBoard_28, NOTATION::CASTLING_RIGHTS::BLACK_SHORT_BIT);
    ASSERT_EQ(be.board, expectedBoard_28);
    ASSERT_EQ(be.hash_, hash::hash(expectedBoard_28));

    // 29
    be.applyMove(notations::uci::createExtendedMove(GIVEN_MOVES[28], be.board.playerOnMove, be.board));
    auto expectedBoard_29 = utils::createBoard(
            "♜  ♛ ♜♚ "
            "♟♘♟♝ ♟♟ "
            "♕    ♞ ♟"
            "  ♟     "
            "   ♟    "
            " ♙  ♙♟  "
            "♙♗♙♙ ♙♙♙"
            "♖   ♔  ♖", NOTATION::COLOR::color::black);
    utils::revokeCastlingRights(expectedBoard_29, NOTATION::CASTLING_RIGHTS::BLACK_LONG_BIT);
    utils::revokeCastlingRights(expectedBoard_29, NOTATION::CASTLING_RIGHTS::BLACK_SHORT_BIT);
    ASSERT_EQ(be.board, expectedBoard_29);
    ASSERT_EQ(be.hash_, hash::hash(expectedBoard_29));

    // 30
    be.applyMove(notations::uci::createExtendedMove(GIVEN_MOVES[29], be.board.playerOnMove, be.board));
    auto expectedBoard_30 = utils::createBoard(
            "♜  ♛ ♜♚ "
            "♟♘♟♝ ♟♟ "
            "♕    ♞ ♟"
            "  ♟     "
            "   ♟    "
            " ♙  ♙   "
            "♙♗♙♙ ♙♟♙"
            "♖   ♔  ♖", NOTATION::COLOR::color::white);
    utils::revokeCastlingRights(expectedBoard_30, NOTATION::CASTLING_RIGHTS::BLACK_LONG_BIT);
    utils::revokeCastlingRights(expectedBoard_30, NOTATION::CASTLING_RIGHTS::BLACK_SHORT_BIT);
    ASSERT_EQ(be.board, expectedBoard_30);
    ASSERT_EQ(be.hash_, hash::hash(expectedBoard_30));

    // 31
    be.applyMove(notations::uci::createExtendedMove(GIVEN_MOVES[30], be.board.playerOnMove, be.board));
    auto expectedBoard_31 = utils::createBoard(
            "♜  ♘ ♜♚ "
            "♟ ♟♝ ♟♟ "
            "♕    ♞ ♟"
            "  ♟     "
            "   ♟    "
            " ♙  ♙   "
            "♙♗♙♙ ♙♟♙"
            "♖   ♔  ♖", NOTATION::COLOR::color::black);
    utils::revokeCastlingRights(expectedBoard_31, NOTATION::CASTLING_RIGHTS::BLACK_LONG_BIT);
    utils::revokeCastlingRights(expectedBoard_31, NOTATION::CASTLING_RIGHTS::BLACK_SHORT_BIT);
    ASSERT_EQ(be.board, expectedBoard_31);
    ASSERT_EQ(be.hash_, hash::hash(expectedBoard_31));

    // 32
    auto move = notations::uci::createExtendedMove(GIVEN_MOVES[31], be.board.playerOnMove, be.board);
    auto expectedMove = ExtendedMove("g2"_field, "h1"_field,
        ExtendedMove::promotionMask | ExtendedMove::pawnMoveMask | ExtendedMove::beatingMask,
        NOTATION::PIECES::QUEEN | NOTATION::COLOR::BLACK,
        NOTATION::PIECES::PAWN | NOTATION::COLOR::BLACK,
        NOTATION::PIECES::ROCK | NOTATION::COLOR::WHITE);
    ASSERT_EQ(move, expectedMove);

    be.applyMove(move);
    auto expectedBoard_32 = utils::createBoard(
            "♜  ♘ ♜♚ "
            "♟ ♟♝ ♟♟ "
            "♕    ♞ ♟"
            "  ♟     "
            "   ♟    "
            " ♙  ♙   "
            "♙♗♙♙ ♙ ♙"
            "♖   ♔  ♛", NOTATION::COLOR::color::white);
    utils::revokeCastlingRights(expectedBoard_32, NOTATION::CASTLING_RIGHTS::BLACK_LONG_BIT);
    utils::revokeCastlingRights(expectedBoard_32, NOTATION::CASTLING_RIGHTS::BLACK_SHORT_BIT);
    ASSERT_EQ(be.board, expectedBoard_32);
    ASSERT_EQ(be.hash_, hash::hash(expectedBoard_32));
}