#include <benchmark/benchmark.h>

#include <notations/uci.hpp>
#include <BoardEngine.hpp>

#include <common/searchingAlgorithms/FullSearchingImplementation.hpp>

static void PerformanceTest_Stability1(benchmark::State& state)
{
	std::vector<std::string> moves = {"b1c3", "e7e6", "g1f3", "d7d5", "e2e4", "d5d4, c3b5", "c7c5", "f3e5", "g8f6", "d1f3", "a7a6", "b5a3", "b7b5", "a3b5", "a6b5", "f1b5", "c8d7", "e5d7", "b8d7", "e4e5", "a8b8", "e5f6", "b8b5", "f3c6", "b5b6", "f6g7", "f8g7", "c6e4", "d7f6", "e4d3", "d8d6", "e1g1", "b6a6", "d3b5", "a6c6", "f1e1", "d4d3", "b5d3", "d6d3", "c2d3", "c6a6", "d3d4", "c5d4", "d2d3", "f6d5", "c1d2", "e8d7", "e1e2", "h8b8", "d2g5", "d7d6", "a2a3", "g7f6", "g5f6", "d5f6", "a1c1", "f6d5", "c1e1", "d6e7", "e2e4", "b8b2", "e4d4", "d5f6", "a3a4", "b2a2", "e1b1", "a6b6", "b1c1", "b6c6", "c1b1", "c6b6", "b1c1", "e7f8", "c1c8", "f8g7", "c8c1", "b6a6", "c1d1", "h7h6", "d4b4", "h6h5", "d3d4", "a6a4", "b4a4", "a2a4", "g1f1", "h5h4", "h2h3", "g7g6", "d1d3", "f6d5", "f1g1", "a4a8", "g1h2", "g6h7", "d3d2", "d5f6", "d2d3", "a8a1", "d3b3", "h7g6", "b3f3", "a1f1", "f3f4", "g6g5", "f4f3", "f1e1", "f3d3", "e1f1", "d3f3", "f1a1", "f3b3", "f6d5", "b3b8", "g5f5", "b8h8", "f5g5", "h8h7", "f7f5", "h7g7", "g5f4", "g7g6", "a1a2"};
	BoardEngine be = {};
	players::common::move_generators::FullCachedEngine cachedEngine;

	for (const auto& moveStr : moves)
    {
        auto move = notations::uci::createExtendedMove(moveStr, be.board.playerOnMove, be.board);
        be.applyMove(move);
    }
    for (auto _ : state)
    {
        auto move = full_search::evaluateIterative(be, cachedEngine, 20);
        benchmark::DoNotOptimize(move);
    }
}

BENCHMARK(PerformanceTest_Stability1);

static void PerformanceTest_Stability2(benchmark::State& state)
{
    std::vector<std::string> moves = {"b1c3", "e7e5", "g1f3", "d7d6", "e2e4", "c7c5", "f1c4", "a7a6", "d2d3", "h7h6", "c1e3", "b7b5", "c4d5", "a8a7", "b2b4", "g8e7", "d5b3", "b8c6", "b4c5", "d6c5", "e3c5", "a7d7", "c5e3", "d8c7", "e3d2", "c6a5", "c3d5", "e7d5", "b3d5", "c8b7", "d5b7", "a5b7", "d1b1", "f7f6", "e1g1", "c7c6", "b1b2", "b7c5", "d2e3", "f8d6", "e3c5", "d6c5", "b2b3", "g7g5", "a2a4", "e8e7", "a4b5", "a6b5", "f1b1", "h8b8", "b1d1", "c6e6", "b3e6", "e7e6", "d1b1", "c5e7", "a1a5", "d7b7", "d3d4", "e7d6", "d4e5", "f6e5"};
	BoardEngine be = {};
	players::common::move_generators::FullCachedEngine cachedEngine;

	for (const auto& moveStr : moves)
    {
        auto move = notations::uci::createExtendedMove(moveStr, be.board.playerOnMove, be.board);
        be.applyMove(move);
    }
    for (auto _ : state)
    {
        auto move = full_search::evaluateIterative(be, cachedEngine, 10);
        benchmark::DoNotOptimize(move);
    }
}
BENCHMARK(PerformanceTest_Stability2); 
