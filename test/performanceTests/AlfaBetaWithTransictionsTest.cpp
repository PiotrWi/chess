#include <gtest/gtest.h>

#include <BoardEngine.hpp>
#include <notations/coordinationnotation.hpp>
#include <common/MoveGenerators/CachedMoveGenerator.hpp>
#include <common/searchingAlgorithms/AlfaBeta.hpp>

TEST(AlfaBetaTrasiction, PerformanceTest_2)
{
    BoardEngine be;
    players::common::move_generators::CachedMoveGenerator mg;
    alfaBeta::evaluate(be, mg, 2);
}

TEST(AlfaBetaTrasiction, PerformanceTest_4)
{
    BoardEngine be;
    players::common::move_generators::CachedMoveGenerator mg;
    alfaBeta::evaluate(be, mg, 4);
}

TEST(AlfaBetaTrasiction, PerformanceTest_6)
{
    BoardEngine be;
    players::common::move_generators::CachedMoveGenerator mg;
    alfaBeta::evaluate(be, mg, 6);
}

TEST(AlfaBetaTrasiction, PerformanceTest_6_black)
{
    BoardEngine be;
    players::common::move_generators::CachedMoveGenerator mg;
    be.applyMove(notations::coordinates::createMove("e2-e4", NOTATION::COLOR::color::white));
    alfaBeta::evaluate(be, mg, 6);
}
/*
TEST(AlfaBetaTrasiction, PerformanceTest_8)
{
    BoardEngine be;
    players::common::move_generators::CachedMoveGenerator mg;
    alfaBeta::evaluate(be, mg, 8);
}

TEST(AlfaBetaTrasiction, PerformanceTest_8_black)
{
    BoardEngine be;
    players::common::move_generators::CachedMoveGenerator mg;
    be.applyMove(notations::coordinates::createMove("e2-e4", NOTATION::COLOR::color::white));
    alfaBeta::evaluate(be, mg, 8);
}
*/