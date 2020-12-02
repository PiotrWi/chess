#include <gtest/gtest.h>

#include <MoveValidator.hpp>

#include <iostream>
#include <regex>

#include <Board.hpp>
#include <utils/BoardGenerationUtils.hpp>

// Long algebraic notation
Move createMove (const std::string& moveStr,
	NOTATION::COLOR::color playerOnMove)
{
	if (moveStr == "O-O")
	{
		if (playerOnMove == NOTATION::COLOR::color::white)
		{
			return Move{"e1", "g1"};
		}
		return Move{"e8", "g8"};
	}
	if (moveStr == "O-O-O")
	{
		if (playerOnMove == NOTATION::COLOR::color::white)
		{
			return Move{"e1", "c1"};
		}
		return Move{"c8", "c8"};
	}

	std::smatch groups;
	std::regex moveTemplate(R"(.*([[:lower:]]\d)[-x]([[:lower:]]\d))");
	auto matched = std::regex_match(moveStr, groups, moveTemplate);

	for (auto i = 0; i < groups.size(); ++i)
	{
		std::cout << groups[i] << std::endl;
	}


	return Move{groups[1].str().c_str(), groups[2].str().c_str()};
}

TEST(MoveValidatorShould, AllowToMovePawn)
{
	Board board = createBoard(
			"♜♞♝♛♚♝♞♜"
			"♟♟♟♟♟♟♟♟"
			"        "
			"        "
			"        "
			"        "
			"♙♙♙♙♙♙♙♙"
			"♖♘♗♔♕♗♘♖");
	auto move = createMove("a2-a4", NOTATION::COLOR::color::white);

	ASSERT_TRUE(MoveValidator::validateMove(board, move));

}
