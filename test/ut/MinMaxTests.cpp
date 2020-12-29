#include <gtest/gtest.h>

#include <BoardEngine.hpp>
#include <common/searchingAlgorithms/MinMax.hpp>

/* measurements
 * i5 6300u     begin       a)          b)
 * depth = 4:   1429ms      1342        1362
 * depth = 6:   935080      906723      836086
 *
 * a - passing Moves to ResultEvaluator to avoid second generation
 * b - Move undoing. Likely for ResultEvaluator it gives some adventage, but for Board it is likely waste(board is relatively small).
 *
 */

TEST(MinMax, PerformanceTest_2)
{
    BoardEngine be;
    minMax::evaluate(be, 2);
}

TEST(MinMax, PerformanceTest_4)
{
    BoardEngine be;
    minMax::evaluate(be, 4);
}
/*
TEST(MinMax, PerformanceTest_6)
{
    BoardEngine be;
    minMax::evaluate(be, 6);
}

TEST(MinMax, PerformanceTest_8)
{
    BoardEngine be;
    minMax::evaluate(be, 8);
}

TEST(MinMax, PerformanceTest_10)
{
    BoardEngine be;
    minMax::evaluate(be, 10);
}

TEST(MinMax, PerformanceTest_12)
{
    BoardEngine be;
    minMax::evaluate(be, 12);
}
*/