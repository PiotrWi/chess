#include <gtest/gtest.h>
#include <utils/BoardGenerationUtils.hpp>
#include <common/evaluators/PawnStructureEvaluator.hpp>

TEST(MaterialEvaluatorShall, EvaluateInitialPosition)
{
    auto initialBoard = utils::createBoard(utils::InitialBoardString,NOTATION::COLOR::color::white);
    ASSERT_EQ(0, pawnStructureEvaluator::evaluateIsolatedPawnsForWhite(initialBoard).first);
    ASSERT_EQ(0, pawnStructureEvaluator::evaluateIsolatedPawnsForBlack(initialBoard).first);
}

TEST(MaterialEvaluatorShall, EvaluateEdges)
{
    const char* boardStr =
            "    ♚   "
            "♟      ♟"
            "        "
            "        "
            "        "
            "        "
            "♙       "
            "    ♔   ";
    auto board = utils::createBoard(boardStr,NOTATION::COLOR::color::white);
    ASSERT_EQ(1, pawnStructureEvaluator::evaluateIsolatedPawnsForWhite(board).first);
    ASSERT_EQ(2, pawnStructureEvaluator::evaluateIsolatedPawnsForBlack(board).first);
}

TEST(MaterialEvaluatorShall, CountDoubledJustOnce)
{
    const char* boardStr =
            "    ♚   "
            "   ♟   ♟"
            "   ♟    "
            "        "
            "   ♙    "
            "        "
            "  ♙♙    "
            "    ♔   ";
    auto board = utils::createBoard(boardStr,NOTATION::COLOR::color::white);
    ASSERT_EQ(0, pawnStructureEvaluator::evaluateIsolatedPawnsForWhite(board).first);
    ASSERT_EQ(2, pawnStructureEvaluator::evaluateIsolatedPawnsForBlack(board).first);
}

TEST(MaterialEvaluatorShall, CountDoubledJustOnce_2)
{
    const char* boardStr =
            "    ♚   "
            "   ♟   ♟"
            "        "
            "      ♟ "
            "   ♙   ♟"
            "   ♟    "
            "   ♙    "
            "    ♔   ";
    auto board = utils::createBoard(boardStr,NOTATION::COLOR::color::white);
    ASSERT_EQ(1, pawnStructureEvaluator::evaluateIsolatedPawnsForWhite(board).first);
    ASSERT_EQ(1, pawnStructureEvaluator::evaluateIsolatedPawnsForBlack(board).first);
}

TEST(MaterialEvaluatorShall, CountDoubledJustOnce_3)
{
    const char* boardStr =
            "    ♚   "
            "   ♟   ♟"
            "        "
            "    ♟   "
            "   ♙   ♟"
            "♙  ♟♙   "
            "♙  ♙  ♟ "
            "    ♔   ";
    auto board = utils::createBoard(boardStr,NOTATION::COLOR::color::white);
    ASSERT_EQ(1, pawnStructureEvaluator::evaluateIsolatedPawnsForWhite(board).first);
    ASSERT_EQ(0, pawnStructureEvaluator::evaluateIsolatedPawnsForBlack(board).first);
}