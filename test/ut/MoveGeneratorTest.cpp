#include <gtest/gtest.h>

#include <MoveGenerator.hpp>
#include <utils/BoardGenerationUtils.hpp>

class MoveGeneratorTests
	: public ::testing::Test
{
public:
	MoveGenerator::MoveGenerator sut;
};

TEST_F(MoveGeneratorTests, shouldReturnAnything)
{
	Board board = utils::createBoard(utils::InitialBoardString);
	sut.generate(board);
}
