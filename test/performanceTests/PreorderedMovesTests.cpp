#include <benchmark/benchmark.h>

#include <vector>
#include <random>

#include <searchingAlgorithms/MoveOrdering.hpp>

#include <utils/BoardGenerationUtils.hpp>
#include <notations/coordinationnotation.hpp>
#include <notations/LongAlgebraicNotation.hpp>

namespace
{

constexpr NOTATION::COLOR::color WHITE = NOTATION::COLOR::color::white;
constexpr NOTATION::COLOR::color BLACK = NOTATION::COLOR::color::black;

const std::vector<ExtendedMove> operator+(const std::vector<ExtendedMove>& lhs,
                                  const std::vector<ExtendedMove>& rhs)
{
    std::vector<ExtendedMove> v;
    v.reserve(lhs.size() + rhs.size());

    v.insert(v.end(), lhs.begin(), lhs.end());
    v.insert(v.end(), rhs.begin(), rhs.end());

    return v;
}

template <size_t N>
std::vector<ExtendedMove> map(const char* (&&in)[N], NOTATION::COLOR::color c, const Board& board)
{
    std::vector<ExtendedMove> moves;
    moves.reserve(N);

    for (auto i = 0u; i < N; ++i)
    {
        moves.emplace_back(notations::long_algebraic::createExtendedMove(in[i], c, board));
    }
    return moves;
}

std::vector<ExtendedMove> randomShuffle(std::vector<ExtendedMove> inOut)
{
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(inOut.begin(), inOut.end(), g);
    return inOut;
}

players::common::move_generators::CacheFullEntity setBestMove(const ExtendedMove& move, int depth)
{
	players::common::move_generators::CacheFullEntity ce;
	ce.previousBestMoves[depth].isSet = true;
	ce.previousBestMoves[depth].move = move;
	return ce;
}

template <typename T>
T noMove(T t)
{
    return t;
}

MoveGenerator::MoveGeneratorV2 getMoveGenerator(const Board& board, NOTATION::COLOR::color c)
{
    MoveGenerator::MoveGeneratorV2 mg(board, c);
    mg.getValidMoveCount();
    return mg;
}

}

static void PerformanceTest_MoveOrdering(benchmark::State& state)
{
    auto board = utils::createBoard("♜   ♚  ♜"
                                    "♟ ♟♟ ♟♟♟"
                                    "♝♟♞ ♟ ♛ "
                                    "   ♙♙   "
                                    "  ♙     "
                                    "♗ ♙  ♘  "
                                    "♙    ♙♙♙"
                                    "♖  ♕ ♖♔ ", BLACK);

    auto bestMove = map({"h7h6"}, BLACK, board);
    auto beatings = map(
            {"e6xd5", "Nc6xe5", "Ba6xc4", "Qg6xg2"}, BLACK, board);
    auto nonBeatings = map({"Ra8b8", "Ra8c8", "Ra8d8", "Rh8g8", "Rh8f8",
    	"O-O-O", "Ke8d8",
    	"b6b5", "d7d6", "f7f6", "f7f5", "h7h5",
    	"Ba6b5", "Ba6b7", "Ba6c8",
    	"Nc6b8", "Nc6d8", "Nc6e7", "Nc6d4", "Nc6b4", "Nc6a5",
    	"Qg6f6", "Qg6h6", "Qg6g5", "Qg6g4", "Qg6g3", "Qg6f5", "Qg6e4", "Qg6d3", "Qg6c2", "Qg6b1", "Qg6h5"}, BLACK, board);

	auto ce = setBestMove(bestMove[0], 3);
	auto allMoves = randomShuffle(beatings) + randomShuffle(nonBeatings + bestMove);

    for (auto _ : state)
    {
        auto mg = getMoveGenerator(board, NOTATION::COLOR::color::white);
        PreorderedMoves pm(NOTATION::COLOR::color::white, &ce, 5, mg, {}, board);
        for (auto i = 0u; i <allMoves.size(); ++i)
        {
            auto moves = pm.get();
        	benchmark::DoNotOptimize(moves);
        }
    }
}

BENCHMARK(PerformanceTest_MoveOrdering);               
