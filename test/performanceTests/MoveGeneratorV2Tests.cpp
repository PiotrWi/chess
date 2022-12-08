#include <benchmark/benchmark.h>

#include <MoveGeneratorV2/MoveGeneratorV2.hpp>

#include <utils/BoardGenerationUtils.hpp>

static void PerformanceTest_MoveGeneratorV2(benchmark::State& state)
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
            MoveGenerator::MoveGeneratorV2 sut(board, NOTATION::COLOR::color::white);
            benchmark::DoNotOptimize(sut.getValidMoveCount());
            benchmark::DoNotOptimize(sut.generateBeatingMoves());
            benchmark::DoNotOptimize(sut.generateNonBeatingMoves());
            benchmark::ClobberMemory();
        }
    }
}
BENCHMARK(PerformanceTest_MoveGeneratorV2);               
