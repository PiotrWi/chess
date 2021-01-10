#include <gtest/gtest.h>

#include <BoardEngine.hpp>
#include <notations/LongAlgebraicNotation.hpp>
#include <common/searchingAlgorithms/AlfaBetaTransictionTableMap.hpp>

TEST(AlfaBetaTrasictionMap, PerformanceTest_2)
{
    BoardEngine be;
    CachedMoveGeneratorMap mg;
    alfaBeta::evaluate(be, mg, 2);
}

TEST(AlfaBetaTrasictionMap, PerformanceTest_4)
{
    BoardEngine be;
    CachedMoveGeneratorMap mg;
    alfaBeta::evaluate(be, mg, 4);
}

TEST(AlfaBetaTrasictionMap, PerformanceTest_6)
{
    BoardEngine be;
    CachedMoveGeneratorMap mg;
    alfaBeta::evaluate(be, mg, 6);
}

TEST(AlfaBetaTrasictionMap, PerformanceTest_6_black)
{
    BoardEngine be;
    CachedMoveGeneratorMap mg;
    be.applyMove(createMove("e2-e4", NOTATION::COLOR::color::white));
    alfaBeta::evaluate(be, mg, 6);
}

TEST(AlfaBetaTrasictionMap, PerformanceTest_8)
{
    BoardEngine be;
    CachedMoveGeneratorMap mg;
    alfaBeta::evaluate(be, mg, 8);
}

TEST(AlfaBetaTrasictionMap, PerformanceTest_8_black)
{
    BoardEngine be;
    CachedMoveGeneratorMap mg;
    be.applyMove(createMove("e2-e4", NOTATION::COLOR::color::white));
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
