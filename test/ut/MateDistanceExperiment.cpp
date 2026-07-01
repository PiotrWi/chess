#include <gtest/gtest.h>

#include <searchingAlgorithms/FullSearchingImplementation.hpp>
#include <utils/BoardGenerationUtils.hpp>
#include <notations/coordinationnotation.hpp>

// bestMove (FullSearchingImplementation.hpp) is a global only ever assigned when a move
// strictly improves alfa or causes a beta cutoff. When the side to move is lost in every line
// and every line scores the same unadjusted mate constant, no move ever strictly improves alfa;
// without a fallback, bestMove would never be touched and full_search::evaluate would silently
// return whatever a *previous, unrelated* call left behind - a move not even legal here.
TEST(MateDistanceExperiment, ReturnsALegalMoveWhenEveryLineIsLostAndTiesAtTheMateConstant)
{
    players::common::move_generators::FullCachedEngine ce;

    // Prime bestMove with a known, unrelated move from a completely different position.
    BoardEngine primer(utils::createBoard("♜♞♝♛♚♝♞♜"
                                          "♟♟♟♟ ♟♟♟"
                                          "        "
                                          "    ♟   "
                                          "  ♗ ♙   "
                                          "     ♕  "
                                          "♙♙♙♙ ♙♙♙"
                                          "♖♘♗ ♔ ♘♖"));
    auto primerMove = full_search::evaluate(primer, ce, 2);
    auto expectedPrimerMove = notations::coordinates::createMove("f3-f7", NOTATION::COLOR::color::white);
    ASSERT_EQ(expectedPrimerMove, primerMove.operator Move());

    // Black king a2, hunted by two white rooks (b7, h4) with the white king on g1: every black
    // king move leads to a forced mate, so every branch ties at the same flat mate constant.
    BoardEngine be(utils::createBoard("        "
                                      " ♖      "
                                      "        "
                                      "        "
                                      "       ♖"
                                      "        "
                                      "♚       "
                                      "      ♔ ", NOTATION::COLOR::color::black));
    ASSERT_EQ(be.getResult(), Result::ongoing);

    auto blackMove = full_search::evaluate(be, ce, 4);

    EXPECT_NE(primerMove, blackMove)
        << "search returned the stale move from the earlier, unrelated position instead of "
           "searching this one";
    EXPECT_EQ(NOTATION::COLOR::COLOR_MASK & blackMove.sourcePiece, NOTATION::COLOR::BLACK)
        << "search returned a move for the wrong side";
}

namespace
{
int g_lastScore = 0;
void captureScore(int alpha, unsigned) { g_lastScore = alpha; }
}  // namespace

TEST(MateDistanceExperiment, ScoresAFasterMateMoreExtremelyThanASlowerOne)
{
    players::common::move_generators::FullCachedEngine ce1;
    BoardEngine mateInOne(utils::createBoard("♜♞♝♛♚♝♞♜"
                                             "♟♟♟♟ ♟♟♟"
                                             "        "
                                             "    ♟   "
                                             "  ♗ ♙   "
                                             "     ♕  "
                                             "♙♙♙♙ ♙♙♙"
                                             "♖♘♗ ♔ ♘♖"));
    full_search::evaluateIterative(mateInOne, ce1, 8, captureScore);
    int scoreMateInOne = g_lastScore;

    // White king + queen vs a bare black king, white to move: a forced win, but it takes more
    // than one move regardless of how black defends.
    players::common::move_generators::FullCachedEngine ce2;
    BoardEngine slowerMate(utils::createBoard("♚       "
                                              "        "
                                              "  ♔     "
                                              "        "
                                              "        "
                                              "        "
                                              "        "
                                              "       ♕"));
    full_search::evaluateIterative(slowerMate, ce2, 8, captureScore);
    int scoreSlowerMate = g_lastScore;

    EXPECT_GT(scoreMateInOne, scoreSlowerMate)
        << "an immediate mate should score more extremely than a mate that takes longer to force";
}
