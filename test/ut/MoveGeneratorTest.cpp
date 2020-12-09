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

}

const std::vector<Move> whitePawnInitialMoves = map(
		{"a2-a3", "a2-a4", "b2-b3", "b2-b4", "c2-c3", "c2-c4", "d2-d3",
		"d2-d4", "e2-e3", "e2-e4", "f2-f3", "f2-f4", "g2-g3", "g2-g4",
		"h2-h3", "h2-h4"}, WHITE);


std::vector<Move> whiteKnightInitialMoves = map(
		{"b1-a3", "b1-c3", "g1-f3", "g1-h3"}, WHITE);

TEST_F(MoveGeneratorTests, shouldReturnPawnMoves)
{
	Board board = utils::createBoard(utils::InitialBoardString);
	ASSERT_THAT(sut.generate(board, WHITE),
		::testing::UnorderedElementsAreArray(whitePawnInitialMoves + whiteKnightInitialMoves));
}
