#include <benchmark/benchmark.h>

#include <MoveGenerator/MoveGenerator.hpp>

#include <utils/BoardGenerationUtils.hpp>

static void PerformanceTest_MoveGenerator(benchmark::State& state)
{
    Board board = utils::createBoard(
                "♜♞..♚♝.."
                "♟♟...♟♟."
                "..♟..♞.♜"
                "...♟♟.♘♟"
                ".♗♗....."
                ".♙♘.♙..."
                "♙.♝♙.♙♙♙"
                "♖...♔..♖"
                , NOTATION::COLOR::color::white);

    for (auto _ : state)
    {
        for (auto i = 0; i < 10; ++i)
        {
            benchmark::DoNotOptimize(MoveGenerator::MoveGenerator::generate(board));
            benchmark::ClobberMemory();
        }
    }
}
BENCHMARK(PerformanceTest_MoveGenerator);               
