#include <future>
#include <thread>

#include <benchmark/benchmark.h>

#include <BoardEngine.hpp>
#include <notations/coordinationnotation.hpp>
#include <common/CachedEngines/FullCachedEngine.hpp>

#include <common/searchingAlgorithms/FullSearchingImplementation.hpp>
#include <utils/BoardGenerationUtils.hpp>

namespace
{

constexpr NOTATION::COLOR::color WHITE = NOTATION::COLOR::color::white;
constexpr NOTATION::COLOR::color BLACK = NOTATION::COLOR::color::black;

}

#define RUN_BENCHMARK_FOR_DEPTH_N(Depth)  static void PerformanceTest_##Depth (benchmark::State& state) \
{                                                                                               \
    BoardEngine be;                                                                             \
    players::common::move_generators::FullCachedEngine mg;                                      \
    for (auto _ : state) {                                                                      \
        benchmark::DoNotOptimize(full_search::evaluate(be, mg, Depth));                         \
    }                                                                                           \
}                                                                                               \
BENCHMARK(PerformanceTest_##Depth );                                                            \

#define RUN_BENCHMARK_FOR_BLACK_DEPTH_N(Depth)  static void PerformanceTest_##Depth##ForBlack (benchmark::State& state) \
{                                                                                                               \
    BoardEngine be;                                                                                             \
    players::common::move_generators::FullCachedEngine mg;                                                      \
    be.applyMove(notations::coordinates::createExtendedMove("e2-e4", NOTATION::COLOR::color::white, be.board)); \
    for (auto _ : state) {                                                                                      \
        benchmark::DoNotOptimize(full_search::evaluate(be, mg, Depth));                                         \
    }                                                                                                           \
}                                                                                                               \
BENCHMARK(PerformanceTest_##Depth##ForBlack );                                                                  \

#define RUN_BENCHMARK_FOR_DEPTH_N_ITERATIVE_SEARCH(Depth)  static void PerformanceTest_##Depth##IterativeSearch (benchmark::State& state) \
{                                                                                                       \
    BoardEngine be;                                                                                     \
    players::common::move_generators::FullCachedEngine mg;                                              \
    for (auto _ : state) {                                                                              \
        benchmark::DoNotOptimize(full_search::evaluateIterative(be, mg, Depth ));                       \
    }                                                                                                   \
}                                                                                                       \
BENCHMARK(PerformanceTest_##Depth##IterativeSearch );  

RUN_BENCHMARK_FOR_DEPTH_N(2)
RUN_BENCHMARK_FOR_DEPTH_N(4)
RUN_BENCHMARK_FOR_DEPTH_N(6)
RUN_BENCHMARK_FOR_DEPTH_N(8)
RUN_BENCHMARK_FOR_DEPTH_N(9)
RUN_BENCHMARK_FOR_BLACK_DEPTH_N(6)
RUN_BENCHMARK_FOR_DEPTH_N_ITERATIVE_SEARCH(2)
RUN_BENCHMARK_FOR_DEPTH_N_ITERATIVE_SEARCH(3)
RUN_BENCHMARK_FOR_DEPTH_N_ITERATIVE_SEARCH(4)
RUN_BENCHMARK_FOR_DEPTH_N_ITERATIVE_SEARCH(5)
RUN_BENCHMARK_FOR_DEPTH_N_ITERATIVE_SEARCH(6)
RUN_BENCHMARK_FOR_DEPTH_N_ITERATIVE_SEARCH(7)
RUN_BENCHMARK_FOR_DEPTH_N_ITERATIVE_SEARCH(8)
RUN_BENCHMARK_FOR_DEPTH_N_ITERATIVE_SEARCH(9)

#define RUN_BENCHMARK_FOR_BLACK_DEPTH_N_CUSTOM(Depth) static void PerformanceTest_##Depth##_custom (benchmark::State& state) \
{                                                                                       \
    Board board = utils::createBoard(                                                   \
                "♜♞..♚♝.."                                                              \
                "♟♟...♟♟."                                                              \
                "..♟..♞.♜"                                                              \
                "...♟♟.♘♟"                                                              \
                ".♗♗....."                                                              \
                ".♙♘.♙..."                                                              \
                "♙.♝♙.♙♙♙"                                                              \
                "♖...♔..♖"                                                              \
                , WHITE);                                                               \
    BoardEngine be;                                                                     \
    be.board = board;                                                                   \
    players::common::move_generators::FullCachedEngine mg;                              \
    for (auto _ : state)                                                                \
    {                                                                                   \
        benchmark::DoNotOptimize(full_search::evaluateIterative(be, mg, Depth ));       \
    }                                                                                   \
    state.counters["nodesPerSecond"] = statistics.getNodesPerSecondLastCalculation();   \
}                                                                                       \
BENCHMARK(PerformanceTest_##Depth##_custom );

RUN_BENCHMARK_FOR_BLACK_DEPTH_N_CUSTOM(6)
RUN_BENCHMARK_FOR_BLACK_DEPTH_N_CUSTOM(7)
RUN_BENCHMARK_FOR_BLACK_DEPTH_N_CUSTOM(8)
RUN_BENCHMARK_FOR_BLACK_DEPTH_N_CUSTOM(9)
/* To slow
RUN_BENCHMARK_FOR_DEPTH_N_ITERATIVE_SEARCH(10)
RUN_BENCHMARK_FOR_DEPTH_N_ITERATIVE_SEARCH(11)
*/

#define RUN_BENCHMARK_FOR_BLACK_DEPTH_N_CUSTOM_WITH_TIME_CONSTRAINT(Depth) static void PerformanceTest_##Depth##_custom_with_time_constraint (benchmark::State& state) \
{                                                                                                       \
    Board board = utils::createBoard(                                                                   \
                "♜♞..♚♝.."                                                                              \
                "♟♟...♟♟."                                                                              \
                "..♟..♞.♜"                                                                              \
                "...♟♟.♘♟"                                                                              \
                ".♗♗....."                                                                              \
                ".♙♘.♙..."                                                                              \
                "♙.♝♙.♙♙♙"                                                                              \
                "♖...♔..♖"                                                                              \
                , WHITE);                                                                               \
    BoardEngine be;                                                                                     \
    be.board = board;                                                                                   \
    players::common::move_generators::FullCachedEngine mg;                                              \
    for (auto _ : state)                                                                                \
    {                                                                                                   \
        auto a = std::async(std::launch::async, [&](){ return full_search::evaluateIterative(be, mg, Depth ); });           \
        std::this_thread::sleep_for(std::chrono::seconds(2));                                           \
        full_search::interrupt();                                                                       \
        a.wait();                                                                                       \
        benchmark::DoNotOptimize(a.get());                                                              \
    }                                                                                                   \
    state.counters["nodesPerSecond"] = statistics.getNodesPerSecondLastCalculation();                   \
}                                                                                                       \
BENCHMARK(PerformanceTest_##Depth##_custom_with_time_constraint );


RUN_BENCHMARK_FOR_BLACK_DEPTH_N_CUSTOM_WITH_TIME_CONSTRAINT(9)