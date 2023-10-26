#include <gtest/gtest.h>
#include <BoardEngine.hpp>
#include <notations/fen.h>
#include <searchingAlgorithms/FullSearchingImplementation.hpp>
#include <CachedEngines/FullCachedEngine.hpp>

const char* fenString = "8/8/8/2r2R2/2k1ppr1/8/1R6/1K6 w - - 2 61";

TEST(WholeGame2, shallProperlyGenerateValidMoves)
{
    BoardEngine be = notations::fen::initByFen(fenString);
    players::common::move_generators::FullCachedEngine ce;

    full_search::evaluateIterative(be, ce, 6);
}

