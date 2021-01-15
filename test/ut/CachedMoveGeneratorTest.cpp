#include <gtest/gtest.h>
#include <common/MoveGenerators/CachedMoveGenerator.hpp>
#include <BoardEngine.hpp>

TEST(CachedMoveGeneratorShall, beReturnList)
{
    BoardEngine be;
    players::common::move_generators::CachedMoveGenerator moveGenerator;
    ASSERT_FALSE(moveGenerator.generate(be).empty());
}
