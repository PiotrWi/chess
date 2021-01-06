#include <gtest/gtest.h>
#include <common/CachedMoveGenerator/CachedMoveGenerator.hpp>
#include <BoardEngine.hpp>

TEST(CachedMoveGeneratorShall, beReturnList)
{
    BoardEngine be;
    CachedMoveGenerator moveGenerator;
    ASSERT_FALSE(moveGenerator.generate(be).empty());
}
