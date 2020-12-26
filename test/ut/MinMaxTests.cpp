#include <gtest/gtest.h>

#include <BoardEngine.hpp>
#include <common/searchingAlgorithms/MinMax.hpp>

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
}*/
/*
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