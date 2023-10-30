#include <gtest/gtest.h>
#include <notations/fen.h>

#include <searchingAlgorithms/FullSearchingImplementation.hpp>

TEST(FenBasedMt, shallNotChrashDurringPosition1)
{
    auto be = notations::fen::initByFen("r2b2k1/r2N1pp1/2p2n1p/p3p1N1/2Q1P3/8/1PP2PPP/R5K1 b - - 0 21");
    players::common::move_generators::FullCachedEngine mg;
    full_search::evaluateIterative(be, mg, 10);
}
