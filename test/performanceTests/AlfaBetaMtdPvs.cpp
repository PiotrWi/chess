#include <gtest/gtest.h>

#include <BoardEngine.hpp>
#include <notations/coordinationnotation.hpp>
#include <common/CachedEngines/CachedEngineWithMap.hpp>

#include <common/searchingAlgorithms/AlfaBetaPvsMtd.hpp>

TEST(alfaBetaPvsMtd, PerformanceTest_2)
{
    BoardEngine be;
    players::common::move_generators::CachedEngineWithMap mg;
    alfaBetaPvsMtd::evaluate(be, mg, 2);
}

TEST(alfaBetaPvsMtd, PerformanceTest_4)
{
    BoardEngine be;
    players::common::move_generators::CachedEngineWithMap mg;
    alfaBetaPvsMtd::evaluate(be, mg, 4);
}

TEST(alfaBetaPvsMtd, PerformanceTest_6)
{
    BoardEngine be;
    players::common::move_generators::CachedEngineWithMap mg;
    alfaBetaPvsMtd::evaluate(be, mg, 6);
}

TEST(alfaBetaPvsMtd, PerformanceTest_6_black)
{
    BoardEngine be;
    players::common::move_generators::CachedEngineWithMap mg;
    be.applyMove(notations::coordinates::createExtendedMove("e2-e4", NOTATION::COLOR::color::white, be.board));
    alfaBetaPvsMtd::evaluate(be, mg, 6);
}

TEST(alfaBetaPvsMtd, PerformanceTest_8)
{
    BoardEngine be;
    players::common::move_generators::CachedEngineWithMap mg;
    alfaBetaPvsMtd::evaluate(be, mg, 8);
}

TEST(alfaBetaPvsMtd, PerformanceTest_8_killer)
{
    BoardEngine be;
    players::common::move_generators::CachedEngineWithMap mg;
    alfaBetaPvsMtd::evaluateIterative(be, mg, 8);
}

TEST(alfaBetaPvsMtd, PerformanceTest_8_black)
{
    BoardEngine be;
    players::common::move_generators::CachedEngineWithMap mg;
    be.applyMove(notations::coordinates::createExtendedMove("e2-e4", NOTATION::COLOR::color::white, be.board));
    alfaBetaPvsMtd::evaluate(be, mg, 8);
}

TEST(alfaBetaPvsMtd, PerformanceTest_8_black_killer)
{
    BoardEngine be;
    players::common::move_generators::CachedEngineWithMap mg;
    alfaBetaPvsMtd::evaluateIterative(be, mg, 8);
}
/*
TEST(alfaBetaPvsMtd, PerformanceTest_10_killer)
{
    BoardEngine be;
    players::common::move_generators::CachedEngineWithMap mg;
    alfaBetaPvsMtd::evaluateIterative(be, mg, 10);
}
 */