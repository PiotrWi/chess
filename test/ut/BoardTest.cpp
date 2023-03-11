#include <gtest/gtest.h>

#include "core/Board.hpp"
#include "core/NotationConversions.hpp"

#include <utils/BoardGenerationUtils.hpp>

TEST(BoardShall, performCorrectlyEquality)
{
    Board a;
    initDefault(a);

    Board b = a;
    ASSERT_EQ(a, b);

    b.validEnPassant = 3;

    ASSERT_FALSE(a == b);

    b = a;
    ++(b.playerOnMove);
    ASSERT_FALSE(a == b);
}

TEST(BoardShall, correctlyIndexFields)
{
    Board a;
    initDefault(a);

    ASSERT_EQ(a.getField("e1"), NOTATION::PIECES::KING | NOTATION::COLOR::WHITE);
    ASSERT_EQ(a.getField("h8"), NOTATION::PIECES::ROCK | NOTATION::COLOR::BLACK);
}

TEST(BoardShall, beEqualToOneCreatedFromString)
{
    Board a;
    initDefault(a);

    auto b = utils::createBoard(utils::InitialBoardString);
    ASSERT_EQ(a, b);
}

TEST(BoardShall, correctlyCleanFields)
{
    Board a;
    initDefault(a);

    a.setField("a1", NOTATION::PIECES::QUEEN | NOTATION::COLOR::WHITE);
    ASSERT_FALSE(a.piecesBitSets[0].rocksMask & 0b1ull);
}

TEST(BoardShall, correctlyCleanFieldsWhenOldValueGiven)
{
    Board a;
    initDefault(a);

    const auto a1_index = 0;
    a.setField(a1_index, NOTATION::PIECES::ROCK | NOTATION::COLOR::WHITE, NOTATION::PIECES::QUEEN | NOTATION::COLOR::WHITE);
    ASSERT_FALSE(a.piecesBitSets[0].rocksMask & 0b1ull);
}