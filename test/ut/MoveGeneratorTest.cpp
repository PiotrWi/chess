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

}

const std::vector<Move> whitePawnInitialMoves = {
		createMove("a2-a3", WHITE),
		createMove("a2-a4", WHITE),
		createMove("b2-b3", WHITE),
		createMove("b2-b4", WHITE),
		createMove("c2-c3", WHITE),
		createMove("c2-c4", WHITE),
		createMove("d2-d3", WHITE),
		createMove("d2-d4", WHITE),
		createMove("e2-e3", WHITE),
		createMove("e2-e4", WHITE),
		createMove("f2-f3", WHITE),
		createMove("f2-f4", WHITE),
		createMove("g2-g3", WHITE),
		createMove("g2-g4", WHITE),
		createMove("h2-h3", WHITE),
		createMove("h2-h4", WHITE)
};

std::vector<Move> whiteKnightInitialMoves = {
		createMove("b1-a3", WHITE),
		createMove("b1-c3", WHITE),
		createMove("g1-f3", WHITE),
		createMove("g1-h3", WHITE),

};

TEST_F(MoveGeneratorTests, shouldReturnPawnMoves)
{
	Board board = utils::createBoard(utils::InitialBoardString);
	ASSERT_THAT(sut.generate(board, WHITE),
		::testing::UnorderedElementsAreArray(whitePawnInitialMoves + whiteKnightInitialMoves));
}
