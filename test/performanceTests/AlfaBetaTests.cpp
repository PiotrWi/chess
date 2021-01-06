#include <gtest/gtest.h>

#include <BoardEngine.hpp>
#include <notations/LongAlgebraicNotation.hpp>
#include <common/searchingAlgorithms/AlfaBeta.hpp>

/* measurements
 * i5 6300u     begin       a)          b)
 * depth = 4:
 * depth = 6:
 *
 *
 */

TEST(AlfaBeta, PerformanceTest_2)
{
    BoardEngine be;
    alfaBeta::evaluate(be, 2);
}

TEST(AlfaBeta, PerformanceTest_4)
{
    BoardEngine be;
    alfaBeta::evaluate(be, 4);
}

TEST(AlfaBeta, PerformanceTest_6)
{
    BoardEngine be;
    alfaBeta::evaluate(be, 6);
}

TEST(AlfaBeta, PerformanceTest_6_black)
{
    BoardEngine be;
    be.applyMove(createMove("e2-e4", NOTATION::COLOR::color::white));
    alfaBeta::evaluate(be, 6);
}

TEST(AlfaBeta, PerformanceTest_8)
{
    BoardEngine be;
    alfaBeta::evaluate(be, 8);
}

TEST(AlfaBeta, PerformanceTest_8_black)
{
    BoardEngine be;
    be.applyMove(createMove("e2-e4", NOTATION::COLOR::color::white));
    alfaBeta::evaluate(be, 8);
}
