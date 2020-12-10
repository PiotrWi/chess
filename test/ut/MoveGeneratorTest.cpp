#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <vector>

#include <MoveGenerator.hpp>
#include <utils/BoardGenerationUtils.hpp>
#include <utils/LongAlgebraicNotation.hpp>

class MoveGeneratorTests
	: public ::testing::Test
{
public:
	MoveGenerator::MoveGenerator sut;
};

constexpr NOTATION::COLOR::color WHITE = NOTATION::COLOR::color::white;

namespace
{

const std::vector<Move> operator+(const std::vector<Move>& lhs,
		const std::vector<Move>& rhs)
{
	std::vector<Move> v;
	v.reserve(lhs.size() + rhs.size());

	v.insert(v.end(), lhs.begin(), lhs.end());
	v.insert(v.end(), rhs.begin(), rhs.end());

	return v;
}

template <size_t N>
std::vector<Move> map(const char* (&&in)[N], NOTATION::COLOR::color c)
{
	std::vector<Move> moves;
	moves.reserve(N);

	for (auto i = 0u; i < N; ++i)
	{
		moves.emplace_back(createMove(in[i], c));
	}
	return moves;
}

}  // namespace

TEST_F(MoveGeneratorTests, shouldReturnPawnMoves)
{
	auto pawnInitialMoves = map(
			{"a2-a3", "a2-a4", "b2-b3", "b2-b4", "c2-c3", "c2-c4", "d2-d3",
			"d2-d4", "e2-e3", "e2-e4", "f2-f3", "f2-f4", "g2-g3", "g2-g4",
			"h2-h3", "h2-h4"}, WHITE);

	auto knightInitialMoves = map(
			{"b1-a3", "b1-c3", "g1-f3", "g1-h3"}, WHITE);

	Board board = utils::createBoard(utils::InitialBoardString);
	ASSERT_THAT(sut.generate(board, WHITE),
		::testing::UnorderedElementsAreArray(pawnInitialMoves + knightInitialMoves));
}

TEST_F(MoveGeneratorTests, shouldCorectlyAnalyzePos_2)
{
	auto pawnMoves = map(
		{"b2-b3", "b2-b4", "c2-c3", "c2-c4", "d4-e5",
		"e4-d5", "e4-f5", "g2-g3", "g2-g4",
		"h2-h3", "h2-h4"}, WHITE);

	auto knightMoves = map(
		{"a3-b5", "a3-c4", "a3-b1",
		"f3-d2", "f3-e5", "f3-g5", "f3-h4", "f3-g1"}, WHITE);

	auto rockMoves = map(
		{"a1-b1", "h1-g1"}, WHITE);

	auto bishopMoves = map(
		{"c1-d2", "c1-e3", "c1-f4", "c1-g5", "c1-h6",
		"f1-e2", "f1-d3", "f1-c4", "f1-b5", "f1-a6"}, WHITE);

	auto queenMoves = map(
		{"d1-d2", "d1-d3", "d1-e2"}, WHITE);

	auto kingMoves = map(
		{"e1-d2", "e1-e2"}, WHITE);

	Board board = utils::createBoard(
		"♜♞♝♛♚♝♞♜"
		"♟♟♟   ♟♟"
		"        "
		"   ♟♟♟  "
		"   ♙♙   "
		"♘    ♘  "
		"♙♙♙  ♙♙♙"
		"♖ ♗♕♔♗ ♖");

	ASSERT_THAT(sut.generate(board, WHITE),
		::testing::UnorderedElementsAreArray(pawnMoves+knightMoves+rockMoves+bishopMoves+queenMoves+kingMoves));
}
