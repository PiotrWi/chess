#include <gtest/gtest.h>

#include <hashing/zobrist.hpp>

#include <utils/BoardGenerationUtils.hpp>

TEST(ZobrishShall, getSameHashForSamePositions)
{
    Board a = utils::createBoard(utils::InitialBoardString);
    Board b = utils::createBoard(utils::InitialBoardString);

    ASSERT_EQ(hash::hash(a), hash::hash(b));
}

TEST(ZobrishShall, getSameHashAfter2ColorChanges)
{
    Board a = utils::createBoard(utils::InitialBoardString);
    auto aHash = hash::hash(a);
    auto bHash = hash::switchColor(aHash);
    auto cHash = hash::switchColor(bHash);

    ASSERT_FALSE(aHash == bHash);
    ASSERT_EQ(aHash, cHash);
}
