#include <gtest/gtest.h>

#include <searchingAlgorithms/FullSearchingImplementation.hpp>

#include <utils/BoardGenerationUtils.hpp>
#include <notations/coordinationnotation.hpp>

TEST(MinMaxAndAlfaBeta, shallFindObviousMate)
{
    players::common::move_generators::FullCachedEngine ce;
    BoardEngine be(utils::createBoard ("♜♞♝♛♚♝♞♜"
                                       "♟♟♟♟ ♟♟♟"
                                       "        "
                                       "    ♟   "
                                       "  ♗ ♙   "
                                       "     ♕  "
                                       "♙♙♙♙ ♙♙♙"
                                       "♖♘♗ ♔ ♘♖"));

    auto expectedMove = notations::coordinates::createMove("f3-f7", NOTATION::COLOR::color::white);
    ASSERT_EQ(expectedMove, full_search::evaluate(be, ce, 2).operator Move());
    ASSERT_EQ(expectedMove, full_search::evaluate(be, ce, 3).operator Move());
    ASSERT_EQ(expectedMove, full_search::evaluate(be, ce, 4).operator Move());
}

TEST(MinMaxAndAlfaBeta, shallFindObviousMateForBlack)
{
    BoardEngine be(utils::createBoard ("    ♚   "
                                       "        "
                                       "        "
                                       "        "
                                       "        "
                                       "       ♞"
                                       "      ♙♙"
                                       "      ♖♔", NOTATION::COLOR::color::black));
    players::common::move_generators::FullCachedEngine ce;

    auto expectedMove = notations::coordinates::createMove("h3-f2", NOTATION::COLOR::color::black);
    ASSERT_EQ(expectedMove, full_search::evaluate(be, ce, 2).operator Move());
    ASSERT_EQ(expectedMove, full_search::evaluate(be, ce, 3).operator Move());
    ASSERT_EQ(expectedMove, full_search::evaluate(be, ce, 4).operator Move());
}
