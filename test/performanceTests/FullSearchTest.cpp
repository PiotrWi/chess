#include <gtest/gtest.h>

#include <BoardEngine.hpp>
#include <notations/coordinationnotation.hpp>
#include <common/CachedEngines/FullCachedEngine.hpp>

#include <common/searchingAlgorithms/FullSearchingImplementation.hpp>

TEST(FullSearchTest, PerformanceTest_2)
{
    BoardEngine be;
    players::common::move_generators::FullCachedEngine mg;
    full_search::evaluate(be, mg, 2);
}

TEST(FullSearchTest, PerformanceTest_4)
{
    BoardEngine be;
    players::common::move_generators::FullCachedEngine mg;
    full_search::evaluate(be, mg, 4);
}

TEST(FullSearchTest, PerformanceTest_6)
{
    BoardEngine be;
    players::common::move_generators::FullCachedEngine mg;
    full_search::evaluate(be, mg, 6);
}

TEST(FullSearchTest, PerformanceTest_6_black)
{
    BoardEngine be;
    players::common::move_generators::FullCachedEngine mg;
    be.applyMove(notations::coordinates::createExtendedMove("e2-e4", NOTATION::COLOR::color::white, be.board));
    full_search::evaluate(be, mg, 6);
}

TEST(FullSearchTest, PerformanceTest_6_iterative)
{
    BoardEngine be;
    players::common::move_generators::FullCachedEngine mg;
    full_search::evaluateIterative(be, mg, 6);
}

TEST(FullSearchTest, PerformanceTest_6_black_iterative)
{
    BoardEngine be;
    players::common::move_generators::FullCachedEngine mg;
    be.applyMove(notations::coordinates::createExtendedMove("e2-e4", NOTATION::COLOR::color::white, be.board));
    full_search::evaluateIterative(be, mg, 6);
}
/*
TEST(FullSearchTest, PerformanceTest_8)
{
    BoardEngine be;
    players::common::move_generators::FullCachedEngine mg;
    full_search::evaluate(be, mg, 8);
}*/

TEST(FullSearchTest, PerformanceTest_8_iterative)
{
    BoardEngine be;
    players::common::move_generators::FullCachedEngine mg;
    full_search::evaluateIterative(be, mg, 8);
}
/*
TEST(FullSearchTest, PerformanceTest_8_black)
{
    BoardEngine be;
    players::common::move_generators::FullCachedEngine mg;
    be.applyMove(notations::coordinates::createMove("e2-e4", NOTATION::COLOR::color::white));
    full_search::evaluate(be, mg, 8);
}
*/
TEST(FullSearchTest, PerformanceTest_8_black_iterative)
{
    BoardEngine be;
    players::common::move_generators::FullCachedEngine mg;
    full_search::evaluateIterative(be, mg, 8);
}

TEST(FullSearchTest, PerformanceTest_10_iterative)
{
    BoardEngine be;
    players::common::move_generators::FullCachedEngine mg;
    full_search::evaluateIterative(be, mg, 10);
}