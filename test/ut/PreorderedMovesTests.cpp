#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <vector>
#include <random>

#include <common/searchingAlgorithms/MoveOrdering.hpp>
#include <utils/BoardGenerationUtils.hpp>
#include <notations/coordinationnotation.hpp>
#include <notations/LongAlgebraicNotation.hpp>

namespace
{

constexpr NOTATION::COLOR::color WHITE = NOTATION::COLOR::color::white;
constexpr NOTATION::COLOR::color BLACK = NOTATION::COLOR::color::black;

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

template <typename T>
T noMove(T t)
{
	return t;
}

std::vector<ExtendedMove> consume_n(PreorderedMoves& pm, int n)
{
	std::vector<ExtendedMove> out;
    std::generate_n(std::back_inserter(out), n, [&pm](){ return *pm.get(); });
    return out;
}

void setHistoryDescending(NOTATION::COLOR::color c, const std::vector<ExtendedMove>& in)
{
	for (unsigned int i = 0; i < in.size(); ++i)
	{
		setHistoryMove(c, in[i], in.size() - i + 1);
	}
}

players::common::move_generators::CacheFullEntity setBestMove(const ExtendedMove& move, int depth)
{
	players::common::move_generators::CacheFullEntity ce;
	ce.previousBestMoves[depth].isSet = true;
	ce.previousBestMoves[depth].move = move;
	return ce;
}

class PreorderedMovesTests
	: public ::testing::Test
{
public:
	void SetUp() override
	{
		clearHistoryMove();
	}

	MoveGenerator::MoveGeneratorV2 getMoveGenerator(const Board& board, NOTATION::COLOR::color c)
	{
		MoveGenerator::MoveGeneratorV2 mg(board, c);
		mg.getValidMoveCount();
		return mg;
	}

	players::common::move_generators::CacheFullEntity emptyCacheEnity_;
};

}  // namespace

TEST_F(PreorderedMovesTests, shouldOrderInitialMoves)
{
    Board board = utils::createBoard(utils::InitialBoardString);
	auto initMoves = map(
            {"a2a3", "a2a4", "b2b3", "b2b4", "c2c3", "c2c4", "d2d3",
            "d2d4", "e2e3", "e2e4", "f2f3", "f2f4", "g2g3", "g2g4",
            "h2h3", "h2h4", "Nb1a3", "Nb1c3", "Ng1f3", "Ng1h3"}, WHITE, board);

	auto mg = getMoveGenerator(board, NOTATION::COLOR::color::white);
    PreorderedMoves pm(NOTATION::COLOR::color::white, &emptyCacheEnity_, 5, mg);
    ASSERT_THAT(consume_n(pm, initMoves.size()), ::testing::UnorderedElementsAreArray(initMoves));
}

TEST_F(PreorderedMovesTests, shouldOrderInitialMovesAccordingToHistory)
{
    Board board = utils::createBoard(utils::InitialBoardString);
	auto movesWithHistory = map({"a2a3", "a2a4", "b2b3", "b2b4", "c2c3", "c2c4", "d2d3"}, WHITE, board);
	setHistoryDescending(NOTATION::COLOR::color::white, movesWithHistory);
	auto movesWithNoHistory = map({"d2d4", "e2e3", "e2e4", "f2f3", "f2f4", "g2g3", "g2g4",
            "h2h3", "h2h4", "Nb1a3", "Nb1c3", "Ng1f3", "Ng1h3"}, WHITE, board);

	auto mg = getMoveGenerator(board, NOTATION::COLOR::color::white);
    PreorderedMoves pm(NOTATION::COLOR::color::white, &emptyCacheEnity_, 5, mg);
    ASSERT_THAT(consume_n(pm, movesWithHistory.size()), ::testing::ElementsAreArray(movesWithHistory));
    ASSERT_THAT(consume_n(pm, movesWithNoHistory.size()), ::testing::UnorderedElementsAreArray(movesWithNoHistory));
}

/*
* TODO to analyze whether depth can be 0;
		If no, CacheFullEntity can be lowered potentially
*/
TEST_F(PreorderedMovesTests, shouldOrderInitialMovesWithBestMoveSet)
{
    Board board = utils::createBoard(utils::InitialBoardString);
    auto bestMove = map({"c2c3"}, WHITE, board);
	auto movesWithHistory = map({"a2a3", "a2a4", "b2b3", "b2b4", "c2c4", "d2d3"}, WHITE, board);
	setHistoryDescending(NOTATION::COLOR::color::white, movesWithHistory);
	auto movesWithNoHistory = map({"d2d4", "e2e3", "e2e4", "f2f3", "f2f4", "g2g3", "g2g4",
            "h2h3", "h2h4", "Nb1a3", "Nb1c3", "Ng1f3", "Ng1h3"}, WHITE, board);
	for (auto depth = 1u; depth <= 5; ++depth)
	{
		auto ce = setBestMove(bestMove[0], depth);
		auto mg = getMoveGenerator(board, NOTATION::COLOR::color::white);
	    PreorderedMoves pm(NOTATION::COLOR::color::white, &ce, 5, mg);

	    ASSERT_THAT(consume_n(pm, bestMove.size()), ::testing::ElementsAreArray(bestMove));
	    ASSERT_THAT(consume_n(pm, movesWithHistory.size()), ::testing::ElementsAreArray(movesWithHistory));
	    ASSERT_THAT(consume_n(pm, movesWithNoHistory.size()), ::testing::UnorderedElementsAreArray(movesWithNoHistory));
	}
}

TEST_F(PreorderedMovesTests, shouldOrderBeatingsByMVVLVA)
{

    auto board = utils::createBoard("♜   ♚  ♜"
                                    "♟ ♟♟ ♟♟♟"
                                    "♝♟♞ ♟ ♛ "
                                    "   ♙♙   "
                                    "  ♙     "
                                    "♗ ♙  ♘  "
                                    "♙    ♙♙♙"
                                    "♖  ♕ ♖♔ ", BLACK);
    auto beatings = map(
            {"e6xd5", "Nc6xe5", "Ba6xc4", "Qg6xg2"}, BLACK, board);
    auto nonBeatings = map({"Ra8b8", "Ra8c8", "Ra8d8", "Rh8g8", "Rh8f8",
    	"O-O-O", "Ke8d8",
    	"b6b5", "d7d6", "f7f6", "f7f5", "h7h6", "h7h5",
    	"Ba6b5", "Ba6b7", "Ba6c8",
    	"Nc6b8", "Nc6d8", "Nc6e7", "Nc6d4", "Nc6b4", "Nc6a5",
    	"Qg6f6", "Qg6h6", "Qg6g5", "Qg6g4", "Qg6g3", "Qg6f5", "Qg6e4", "Qg6d3", "Qg6c2", "Qg6b1", "Qg6h5"}, BLACK, board);

    auto mg = getMoveGenerator(board, BLACK);
	PreorderedMoves pm(NOTATION::COLOR::color::white, &emptyCacheEnity_, 5, mg);

	ASSERT_THAT(consume_n(pm, beatings.size()), ::testing::ElementsAreArray(beatings));
	ASSERT_THAT(consume_n(pm, nonBeatings.size()), ::testing::UnorderedElementsAreArray(nonBeatings));
}

TEST_F(PreorderedMovesTests, shouldOrderBeatingsByMVVLVA_WithBestMoveSetAsBeating)
{

    auto board = utils::createBoard("♜   ♚  ♜"
                                    "♟ ♟♟ ♟♟♟"
                                    "♝♟♞ ♟ ♛ "
                                    "   ♙♙   "
                                    "  ♙     "
                                    "♗ ♙  ♘  "
                                    "♙    ♙♙♙"
                                    "♖  ♕ ♖♔ ", BLACK);
    auto bestMove = map({"Qg6xg2"}, BLACK, board);
    auto beatings = map(
            {"e6xd5", "Nc6xe5", "Ba6xc4"}, BLACK, board);
    auto nonBeatings = map({"Ra8b8", "Ra8c8", "Ra8d8", "Rh8g8", "Rh8f8",
    	"O-O-O", "Ke8d8",
    	"b6b5", "d7d6", "f7f6", "f7f5", "h7h6", "h7h5",
    	"Ba6b5", "Ba6b7", "Ba6c8",
    	"Nc6b8", "Nc6d8", "Nc6e7", "Nc6d4", "Nc6b4", "Nc6a5",
    	"Qg6f6", "Qg6h6", "Qg6g5", "Qg6g4", "Qg6g3", "Qg6f5", "Qg6e4", "Qg6d3", "Qg6c2", "Qg6b1", "Qg6h5"}, BLACK, board);

	auto ce = setBestMove(bestMove[0], 3);
	auto mg = getMoveGenerator(board, NOTATION::COLOR::color::black);
	PreorderedMoves pm(NOTATION::COLOR::color::white, &ce, 5, mg);

	ASSERT_THAT(consume_n(pm, bestMove.size()), ::testing::ElementsAreArray(bestMove));
	ASSERT_THAT(consume_n(pm, beatings.size()), ::testing::ElementsAreArray(beatings));
	ASSERT_THAT(consume_n(pm, nonBeatings.size()), ::testing::UnorderedElementsAreArray(nonBeatings));
}

TEST_F(PreorderedMovesTests, shouldOrderBeatingsByMVVLVA_WithBestMoveSetAsNonBeating)
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
    {
		auto ce = setBestMove(bestMove[0], 3);
		auto mg = getMoveGenerator(board, NOTATION::COLOR::color::black);
		PreorderedMoves pm(NOTATION::COLOR::color::white, &ce, 5, mg);

		ASSERT_THAT(consume_n(pm, bestMove.size()), ::testing::ElementsAreArray(bestMove));
		ASSERT_THAT(consume_n(pm, beatings.size()), ::testing::ElementsAreArray(beatings));
		ASSERT_THAT(consume_n(pm, nonBeatings.size()), ::testing::UnorderedElementsAreArray(nonBeatings));
    }
    {
		auto ce = setBestMove(bestMove[0], 3);
		auto mg = getMoveGenerator(board, NOTATION::COLOR::color::black);
		PreorderedMoves pm(NOTATION::COLOR::color::white, &ce, 5, mg);

		ASSERT_THAT(consume_n(pm, bestMove.size()), ::testing::ElementsAreArray(bestMove));
		ASSERT_THAT(consume_n(pm, beatings.size()), ::testing::ElementsAreArray(beatings));
		ASSERT_THAT(consume_n(pm, nonBeatings.size()), ::testing::UnorderedElementsAreArray(nonBeatings));
    }
    {
		auto ce = setBestMove(bestMove[0], 3);
		auto mg = getMoveGenerator(board, NOTATION::COLOR::color::black);
		PreorderedMoves pm(NOTATION::COLOR::color::white, &ce, 5, mg);

		ASSERT_THAT(consume_n(pm, bestMove.size()), ::testing::ElementsAreArray(bestMove));
		ASSERT_THAT(consume_n(pm, beatings.size()), ::testing::ElementsAreArray(beatings));
		ASSERT_THAT(consume_n(pm, nonBeatings.size()), ::testing::UnorderedElementsAreArray(nonBeatings));
    }
}
