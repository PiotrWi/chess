#include <gtest/gtest.h>

#include <BoardEngine.hpp>
#include <notations/coordinationnotation.hpp>
#include <common/CachedEngines/CachedEngineWithMap.hpp>
#include <common/CachedEngines/MoveGenerator.hpp>

#include <common/searchingAlgorithms/AlfaBetaPvs.hpp>

TEST(AlfaBetaPvs, PerformanceTest_2)
{
    BoardEngine be;
    players::common::move_generators::CachedEngineWithMap mg;
    alfaBetaPvs::evaluate(be, mg, 2);
}

TEST(AlfaBetaPvs, PerformanceTest_4)
{
    BoardEngine be;
    players::common::move_generators::CachedEngineWithMap mg;
    alfaBetaPvs::evaluate(be, mg, 4);
}

TEST(AlfaBetaPvs, PerformanceTest_6)
{
    BoardEngine be;
    players::common::move_generators::CachedEngineWithMap mg;
    alfaBetaPvs::evaluate(be, mg, 6);
}

TEST(AlfaBetaPvs, PerformanceTest_6_black)
{
    BoardEngine be;
    players::common::move_generators::CachedEngineWithMap mg;
    be.applyMove(notations::coordinates::createExtendedMove("e2-e4", NOTATION::COLOR::color::white, be.board));
    alfaBetaPvs::evaluate(be, mg, 6);
}

TEST(AlfaBetaPvs, PerformanceTest_8)
{
    BoardEngine be;
    players::common::move_generators::CachedEngineWithMap mg;
    alfaBetaPvs::evaluate(be, mg, 8);
}

TEST(AlfaBetaPvs, PerformanceTest_8_killer)
{
    BoardEngine be;
    players::common::move_generators::CachedEngineWithMap mg;
    alfaBetaPvs::evaluateIterative(be, mg, 8);
}

TEST(AlfaBetaPvs, PerformanceTest_8_black)
{
    BoardEngine be;
    players::common::move_generators::CachedEngineWithMap mg;
    be.applyMove(notations::coordinates::createExtendedMove("e2-e4", NOTATION::COLOR::color::white, be.board));
    alfaBetaPvs::evaluate(be, mg, 8);
}

TEST(AlfaBetaPvs, PerformanceTest_8_black_killer)
{
    BoardEngine be;
    players::common::move_generators::CachedEngineWithMap mg;
    alfaBetaPvs::evaluateIterative(be, mg, 8);
}

TEST(AlfaBetaPvsNoCache, PerformanceTest_8)
{
    BoardEngine be;
    players::common::move_generators::MoveGenerator mg;
    alfaBetaPvs::evaluate(be, mg, 8);
}

TEST(AlfaBetaPvsNoCache, PerformanceTest_8_black)
{
    BoardEngine be;
    players::common::move_generators::MoveGenerator mg;
    be.applyMove(notations::coordinates::createExtendedMove("e2-e4", NOTATION::COLOR::color::white, be.board));
    alfaBetaPvs::evaluate(be, mg, 8);
}
