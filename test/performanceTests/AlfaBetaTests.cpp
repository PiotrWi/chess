#include <gtest/gtest.h>

#include <BoardEngine.hpp>
#include <notations/coordinationnotation.hpp>
#include <common/searchingAlgorithms/AlfaBeta.hpp>
#include <common/CachedEngines/MoveGenerator.hpp>

players::common::move_generators::MoveGenerator mg;

TEST(AlfaBeta, PerformanceTest_2)
{
    BoardEngine be;
    alfaBeta::evaluate(be, mg, 2);
}

TEST(AlfaBeta, PerformanceTest_4)
{
    BoardEngine be;
    alfaBeta::evaluate(be, mg, 4);
}

TEST(AlfaBeta, PerformanceTest_6)
{
    BoardEngine be;
    alfaBeta::evaluate(be, mg, 6);
}

TEST(AlfaBeta, PerformanceTest_6_black)
{
    BoardEngine be;
    be.applyMove(notations::coordinates::createExtendedMove("e2-e4", NOTATION::COLOR::color::white, be.board));
    alfaBeta::evaluate(be, mg, 6);
}
