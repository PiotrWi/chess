#include <gtest/gtest.h>
#include <notations/fen.h>
#include <utils/BoardGenerationUtils.hpp>

TEST(FenShall, CorrectlyReadInitialPosition)
{
    Board expectedBoard = utils::createBoard(utils::InitialBoardString);

    auto be = notations::coordinates::initByFen(std::string("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"));

    ASSERT_EQ(expectedBoard, be.board);
}

TEST(FenShall, CorrectlyReadInitialPosition_butForBlack)
{
    Board expectedBoard = utils::createBoard(utils::InitialBoardString, NOTATION::COLOR::color::black);

    auto be = notations::coordinates::initByFen(std::string("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1"));

    ASSERT_EQ(expectedBoard, be.board);
}

TEST(FenShall, CorrectlyReadInitialPosition_RuiLopes)
{
    Board expectedBoard = utils::createBoard("♜ ♝♛♚♝ ♜"
                                             " ♟♟♟ ♟♟♟"
                                             "♟ ♞  ♞  "
                                             "    ♟   "
                                             "♗   ♙   "
                                             "     ♘  "
                                             "♙♙♙♙ ♙♙♙"
                                             "♖♘♗♕♔  ♖");

    auto be = notations::coordinates::initByFen(std::string("r1bqkb1r/1ppp1ppp/p1n2n2/4p3/B3P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 1"));

    ASSERT_EQ(expectedBoard, be.board);
}
