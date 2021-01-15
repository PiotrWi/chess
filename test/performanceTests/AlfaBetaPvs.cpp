#include <gtest/gtest.h>

#include <BoardEngine.hpp>
#include <notations/coordinationnotation.hpp>
#include <common/MoveGenerators/CachedMoveGeneratorMap.hpp>
#include <common/searchingAlgorithms/AlfaBetaPvs.hpp>

TEST(AlfaBetaPvs, PerformanceTest_2)
{
    BoardEngine be;
    players::common::move_generators::CachedMoveGeneratorMap mg;
    alfaBetaPvs::evaluate(be, mg, 2);
}

TEST(AlfaBetaPvs, PerformanceTest_4)
{
    BoardEngine be;
    players::common::move_generators::CachedMoveGeneratorMap mg;
    alfaBetaPvs::evaluate(be, mg, 4);
}

TEST(AlfaBetaPvs, PerformanceTest_6)
{
    BoardEngine be;
    players::common::move_generators::CachedMoveGeneratorMap mg;
    alfaBetaPvs::evaluate(be, mg, 6);
}

TEST(AlfaBetaPvs, PerformanceTest_6_black)
{
    BoardEngine be;
    players::common::move_generators::CachedMoveGeneratorMap mg;
    be.applyMove(notations::coordinates::createMove("e2-e4", NOTATION::COLOR::color::white));
    alfaBetaPvs::evaluate(be, mg, 6);
}

TEST(AlfaBetaPvs, PerformanceTest_8)
{
    BoardEngine be;
    players::common::move_generators::CachedMoveGeneratorMap mg;
    alfaBetaPvs::evaluate(be, mg, 8);
}

TEST(AlfaBetaPvs, PerformanceTest_8_killer)
{
    BoardEngine be;
    players::common::move_generators::CachedMoveGeneratorMap mg;
    alfaBetaPvs::evaluate(be, mg, 2);
    alfaBetaPvs::evaluate(be, mg, 4);
    alfaBetaPvs::evaluate(be, mg, 6);
    alfaBetaPvs::evaluate(be, mg, 8);
}

TEST(AlfaBetaPvs, PerformanceTest_8_black)
{
    BoardEngine be;
    players::common::move_generators::CachedMoveGeneratorMap mg;
    be.applyMove(notations::coordinates::createMove("e2-e4", NOTATION::COLOR::color::white));
    alfaBetaPvs::evaluate(be, mg, 8);
}

/*TEST(AlfaBetaTrasictionMap, PerformanceTest_10)
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
