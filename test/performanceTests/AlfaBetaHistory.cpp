#include <gtest/gtest.h>

#include <BoardEngine.hpp>
#include <notations/coordinationnotation.hpp>
#include <common/CachedEngines/CachedEngineWithMap.hpp>
#include <common/CachedEngines/MoveGenerator.hpp>

#include <common/searchingAlgorithms/AlfaBetaPvsHistory.hpp>

TEST(alfaBetaPvsHistory, PerformanceTest_2)
{
    BoardEngine be;
    players::common::move_generators::CachedEngineWithMap mg;
    alfaBetaPvsHistory::evaluate(be, mg, 2);
}

TEST(alfaBetaPvsHistory, PerformanceTest_4)
{
    BoardEngine be;
    players::common::move_generators::CachedEngineWithMap mg;
    alfaBetaPvsHistory::evaluate(be, mg, 4);
}

TEST(alfaBetaPvsHistory, PerformanceTest_6)
{
    BoardEngine be;
    players::common::move_generators::CachedEngineWithMap mg;
    alfaBetaPvsHistory::evaluate(be, mg, 6);
}

TEST(alfaBetaPvsHistory, PerformanceTest_6_black)
{
    BoardEngine be;
    players::common::move_generators::CachedEngineWithMap mg;
    be.applyMove(notations::coordinates::createMove("e2-e4", NOTATION::COLOR::color::white));
    alfaBetaPvsHistory::evaluate(be, mg, 6);
}

TEST(alfaBetaPvsHistory, PerformanceTest_8)
{
    BoardEngine be;
    players::common::move_generators::CachedEngineWithMap mg;
    alfaBetaPvsHistory::evaluate(be, mg, 8);
}

TEST(alfaBetaPvsHistory, PerformanceTest_8_killer)
{
    BoardEngine be;
    players::common::move_generators::CachedEngineWithMap mg;
    alfaBetaPvsHistory::evaluateIterative(be, mg, 8);
}

TEST(alfaBetaPvsHistory, PerformanceTest_8_black)
{
    BoardEngine be;
    players::common::move_generators::CachedEngineWithMap mg;
    be.applyMove(notations::coordinates::createMove("e2-e4", NOTATION::COLOR::color::white));
    alfaBetaPvsHistory::evaluate(be, mg, 8);
}

TEST(alfaBetaPvsHistory, PerformanceTest_8_black_killer)
{
    BoardEngine be;
    players::common::move_generators::CachedEngineWithMap mg;
    alfaBetaPvsHistory::evaluateIterative(be, mg, 8);
}
/*
TEST(alfaBetaPvsHistory, PerformanceTest_10_killer)
{
    BoardEngine be;
    players::common::move_generators::CachedEngineWithMap mg;
    alfaBetaPvsHistory::evaluateIterative(be, mg, 10);
}
 */