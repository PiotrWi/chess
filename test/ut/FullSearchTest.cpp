#include <common/searchingAlgorithms/FullSearchingImplementation.hpp>

#include <gtest/gtest.h>

TEST(MVVLVA_ComparatorShall, placeBestMovesAtFront)
{
    auto pawnBeatQueen = ExtendedMove(0, 1, ExtendedMove::beatingMask, 0, NOTATION::PIECES::PAWN, NOTATION::PIECES::QUEEN);
    auto pawnPromoteAndBeatRock = ExtendedMove(0, 1, ExtendedMove::beatingMask | ExtendedMove::promotionMask, 0, NOTATION::PIECES::PAWN, NOTATION::PIECES::ROCK);
    auto rockBeatRock = ExtendedMove(0, 1, ExtendedMove::beatingMask, 0, NOTATION::PIECES::ROCK, NOTATION::PIECES::ROCK);

    std::vector<ExtendedMove> in{
            pawnBeatQueen, pawnPromoteAndBeatRock ,rockBeatRock
    };

    std::vector<ExtendedMove> sorted = {pawnPromoteAndBeatRock, pawnBeatQueen, rockBeatRock};
    std::sort(in.begin(), in.end(), MVVLVA_Comparator::compare);

    ASSERT_EQ(in, sorted);
}