#include <gtest/gtest.h>

#include <publicIf/Board.hpp>
#include <publicIf/NotationConversions.hpp>

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

    ASSERT_EQ(a["e1"], NOTATION::PIECES::KING | NOTATION::COLOR::WHITE);
    ASSERT_EQ(a["h8"], NOTATION::PIECES::ROCK | NOTATION::COLOR::BLACK);
}