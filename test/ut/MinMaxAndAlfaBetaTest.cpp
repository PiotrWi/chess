#include <gtest/gtest.h>

#include <common/searchingAlgorithms/AlfaBeta.hpp>
#include <common/searchingAlgorithms/MinMax.hpp>

#include <utils/BoardGenerationUtils.hpp>
#include <notations/LongAlgebraicNotation.hpp>

TEST(MinMaxAndAlfaBeta, shallReturnSameValues)
{
    BoardEngine be;
    ASSERT_EQ(minMax::evaluate(be, 2), alfaBeta::evaluate(be, 2));
    ASSERT_EQ(minMax::evaluate(be, 3), alfaBeta::evaluate(be, 3));
    ASSERT_EQ(minMax::evaluate(be, 4), alfaBeta::evaluate(be, 4));

    be.applyMove(createMove("e2-e4", NOTATION::COLOR::color::white));
    ASSERT_EQ(minMax::evaluate(be, 2), alfaBeta::evaluate(be, 2));
    ASSERT_EQ(minMax::evaluate(be, 3), alfaBeta::evaluate(be, 3));
}

TEST(MinMaxAndAlfaBeta, TEMP_shallReturnSameValues)
{
    BoardEngine be;
    // WHY? Move is different O_o
    be.applyMove(createMove("e2-e4", NOTATION::COLOR::color::white));
    ASSERT_EQ(minMax::evaluate(be, 4), alfaBeta::evaluate(be, 4));
}

TEST(MinMaxAndAlfaBeta, shallFindObviousMate)
{
    BoardEngine be;
    be.board = utils::createBoard ("♜♞♝♛♚♝♞♜"
                                   "♟♟♟♟ ♟♟♟"
                                   "        "
                                   "    ♟   "
                                   "  ♗ ♙   "
                                   "     ♕  "
                                   "♙♙♙♙ ♙♙♙"
                                   "♖♘♗ ♔ ♘♖");

    auto expectedMove = createMove("f3-f7", NOTATION::COLOR::color::white);
    ASSERT_EQ(expectedMove, minMax::evaluate(be, 2));
    ASSERT_EQ(expectedMove, minMax::evaluate(be, 3));
    // ASSERT_EQ(expectedMove, minMax::evaluate(be, 4));

    ASSERT_EQ(expectedMove, alfaBeta::evaluate(be, 2));
    ASSERT_EQ(expectedMove, alfaBeta::evaluate(be, 3));
    ASSERT_EQ(expectedMove, alfaBeta::evaluate(be, 4));
}

TEST(MinMaxAndAlfaBeta, shallFindObviousMateForBlack)
{
    BoardEngine be;
    be.board = utils::createBoard ("    ♚   "
                                   "        "
                                   "        "
                                   "        "
                                   "        "
                                   "       ♞"
                                   "      ♙♙"
                                   "      ♖♔", NOTATION::COLOR::color::black);

    auto expectedMove = createMove("h3-f2", NOTATION::COLOR::color::black);
    ASSERT_EQ(expectedMove, minMax::evaluate(be, 2));
    ASSERT_EQ(expectedMove, minMax::evaluate(be, 3));
    ASSERT_EQ(expectedMove, minMax::evaluate(be, 4));

    ASSERT_EQ(expectedMove, alfaBeta::evaluate(be, 2));
    ASSERT_EQ(expectedMove, alfaBeta::evaluate(be, 3));
    ASSERT_EQ(expectedMove, alfaBeta::evaluate(be, 4));
}

TEST(MinMaxAndAlfaBeta, shallSolvePuzzle)
{
    BoardEngine be;
    be.board = utils::createBoard ("  ♜     "
                                   " ♞   ♟♚ "
                                   "♟♝ ♟  ♟ "
                                   "   ♙♟  ♟"
                                   "    ♙  ♙"
                                   "♙    ♕♙♛"
                                   "  ♗  ♙  "
                                   "   ♖♘ ♔ ", NOTATION::COLOR::color::white);

    auto expectedMove = createMove("d1-b1", NOTATION::COLOR::color::white);
    ASSERT_EQ(expectedMove, minMax::evaluate(be, 4));

    ASSERT_EQ(expectedMove, alfaBeta::evaluate(be, 6));
}

TEST(MinMaxAndAlfaBeta, shallSolvePuzzle_2)
{
    BoardEngine be;
    be.board = utils::createBoard ("  ♜   ♚ "
                                   " ♛   ♟♟ "
                                   "    ♟  ♟"
                                   "        "
                                   "♟ ♙  ♖ ♙"
                                   "♙     ♙ "
                                   "  ♗  ♙  "
                                   "     ♕♔ ", NOTATION::COLOR::color::black);

    auto expectedMove = createMove("e6-e5", NOTATION::COLOR::color::black);
    ASSERT_EQ(expectedMove, minMax::evaluate(be, 4));

    ASSERT_EQ(expectedMove, alfaBeta::evaluate(be, 6));
}