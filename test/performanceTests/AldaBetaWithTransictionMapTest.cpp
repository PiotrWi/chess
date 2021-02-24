#include <gtest/gtest.h>

#include <BoardEngine.hpp>
#include <notations/coordinationnotation.hpp>
#include <common/CachedEngines/CachedEngineWithMap.hpp>
#include <common/searchingAlgorithms/AlfaBeta.hpp>

TEST(AlfaBetaTrasictionMap, PerformanceTest_2)
{
    BoardEngine be;
    players::common::move_generators::CachedEngineWithMap mg;
    alfaBeta::evaluate(be, mg, 2);
}

TEST(AlfaBetaTrasictionMap, PerformanceTest_4)
{
    BoardEngine be;
    players::common::move_generators::CachedEngineWithMap mg;
    alfaBeta::evaluate(be, mg, 4);
}

TEST(AlfaBetaTrasictionMap, PerformanceTest_6)
{
    BoardEngine be;
    players::common::move_generators::CachedEngineWithMap mg;
    alfaBeta::evaluate(be, mg, 6);
}

TEST(AlfaBetaTrasictionMap, PerformanceTest_6_black)
{
    BoardEngine be;
    players::common::move_generators::CachedEngineWithMap mg;
    be.applyMove(notations::coordinates::createExtendedMove("e2-e4", NOTATION::COLOR::color::white, be.board));
    alfaBeta::evaluate(be, mg, 6);
}

TEST(AlfaBetaTrasictionMap, PerformanceTest_8)
{
    BoardEngine be;
    players::common::move_generators::CachedEngineWithMap mg;
    alfaBeta::evaluate(be, mg, 8);
}

TEST(AlfaBetaTrasictionMap, PerformanceTest_8_killer)
{
    BoardEngine be;
    players::common::move_generators::CachedEngineWithMap mg;
    alfaBeta::evaluate(be, mg, 2);
    alfaBeta::evaluate(be, mg, 4);
    alfaBeta::evaluate(be, mg, 6);
    alfaBeta::evaluate(be, mg, 8);
}

/*
TEST(AlfaBetaTrasictionMap, PerformanceTest_10_black)
{
    BoardEngine be;
    CachedEngineWithMap mg;
    be.applyMove(createMove("e2-e4", NOTATION::COLOR::color::white));
    alfaBeta::evaluate(be, mg, 10);
}*/
