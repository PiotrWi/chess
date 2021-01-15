#include <gtest/gtest.h>

#include <BoardEngine.hpp>
#include <notations/coordinationnotation.hpp>
#include <common/MoveGenerators/CachedMoveGeneratorMap.hpp>
#include <common/searchingAlgorithms/AlfaBeta.hpp>

TEST(AlfaBetaTrasictionMap, PerformanceTest_2)
{
    BoardEngine be;
    players::common::move_generators::CachedMoveGeneratorMap mg;
    mg.makeOlder();
    mg.clearOlderThan(5u);
    alfaBeta::evaluate(be, mg, 2);
    mg.makeOlder();
    mg.clearOlderThan(5u);
}

TEST(AlfaBetaTrasictionMap, PerformanceTest_4)
{
    BoardEngine be;
    players::common::move_generators::CachedMoveGeneratorMap mg;
    alfaBeta::evaluate(be, mg, 4);
}

TEST(AlfaBetaTrasictionMap, PerformanceTest_6)
{
    BoardEngine be;
    players::common::move_generators::CachedMoveGeneratorMap mg;
    alfaBeta::evaluate(be, mg, 6);
}

TEST(AlfaBetaTrasictionMap, PerformanceTest_6_black)
{
    BoardEngine be;
    players::common::move_generators::CachedMoveGeneratorMap mg;
    be.applyMove(notations::coordinates::createMove("e2-e4", NOTATION::COLOR::color::white));
    alfaBeta::evaluate(be, mg, 6);
}

TEST(AlfaBetaTrasictionMap, PerformanceTest_8)
{
    BoardEngine be;
    players::common::move_generators::CachedMoveGeneratorMap mg;
    alfaBeta::evaluate(be, mg, 8);
}

TEST(AlfaBetaTrasictionMap, PerformanceTest_8_black)
{
    BoardEngine be;
    players::common::move_generators::CachedMoveGeneratorMap mg;
    be.applyMove(notations::coordinates::createMove("e2-e4", NOTATION::COLOR::color::white));
    alfaBeta::evaluate(be, mg, 8);
}
/*
TEST(AlfaBetaTrasictionMap, PerformanceTest_10)
{
    BoardEngine be;
    CachedMoveGeneratorMap mg;
    alfaBeta::evaluate(be, mg, 10);
}

TEST(AlfaBetaTrasictionMap, PerformanceTest_10_black)
{
    BoardEngine be;
    CachedMoveGeneratorMap mg;
    be.applyMove(createMove("e2-e4", NOTATION::COLOR::color::white));
    alfaBeta::evaluate(be, mg, 10);
}*/
