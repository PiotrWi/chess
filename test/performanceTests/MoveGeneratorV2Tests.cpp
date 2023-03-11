#include <benchmark/benchmark.h>

#include <MoveGeneratorV2/MoveGeneratorV2.hpp>

#include <utils/BoardGenerationUtils.hpp>

static void PerformanceTest_MoveGeneratorV2_justMoveCount(benchmark::State& state)
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
            auto moveCount = sut.getValidMoveCount();
            benchmark::DoNotOptimize(moveCount);
            benchmark::ClobberMemory();
        }
    }
}
BENCHMARK(PerformanceTest_MoveGeneratorV2_justMoveCount);

static void PerformanceTest_MoveGeneratorV2_MoveCountAndBeatings(benchmark::State& state)
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
            auto moveCount = sut.getValidMoveCount();
            benchmark::DoNotOptimize(moveCount);
            auto beatingMoves = sut.generateBeatingMoves();
            benchmark::DoNotOptimize(beatingMoves);
            benchmark::ClobberMemory();
        }
    }
}
BENCHMARK(PerformanceTest_MoveGeneratorV2_MoveCountAndBeatings);

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
            auto moveCount = sut.getValidMoveCount();
            benchmark::DoNotOptimize(moveCount);
            auto beatingMoves = sut.generateBeatingMoves();
            benchmark::DoNotOptimize(beatingMoves);
            auto nonBeatingMoves = sut.generateBeatingMoves();
            benchmark::DoNotOptimize(nonBeatingMoves);
            benchmark::ClobberMemory();
        }
    }
}
BENCHMARK(PerformanceTest_MoveGeneratorV2);               
