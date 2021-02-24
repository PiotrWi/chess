#include <gtest/gtest.h>

#include <common/searchingAlgorithms/AlfaBeta.hpp>
#include <common/searchingAlgorithms/MinMax.hpp>

#include <utils/BoardGenerationUtils.hpp>
#include <notations/coordinationnotation.hpp>

#include <common/CachedEngines/MoveGenerator.hpp>

players::common::move_generators::MoveGenerator mg;

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

    auto expectedMove = notations::coordinates::createMove("f3-f7", NOTATION::COLOR::color::white);
    ASSERT_EQ(expectedMove, minMax::evaluate(be, 2));
    ASSERT_EQ(expectedMove, minMax::evaluate(be, 3));

    ASSERT_EQ(expectedMove, alfaBeta::evaluate(be, mg, 2));
    ASSERT_EQ(expectedMove, alfaBeta::evaluate(be, mg, 3));
    ASSERT_EQ(expectedMove, alfaBeta::evaluate(be, mg, 4));
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

    auto expectedMove = notations::coordinates::createMove("h3-f2", NOTATION::COLOR::color::black);
    ASSERT_EQ(expectedMove, minMax::evaluate(be, 2));
    ASSERT_EQ(expectedMove, minMax::evaluate(be, 3));
    ASSERT_EQ(expectedMove, minMax::evaluate(be, 4));

    ASSERT_EQ(expectedMove, alfaBeta::evaluate(be, mg, 2));
    ASSERT_EQ(expectedMove, alfaBeta::evaluate(be, mg, 3));
    ASSERT_EQ(expectedMove, alfaBeta::evaluate(be, mg, 4));
}
