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

TEST(MinMax, PerformanceTest_5)
{
    BoardEngine be;
    minMax::evaluate(be, 5);
}