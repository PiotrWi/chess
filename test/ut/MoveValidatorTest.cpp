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

	return Move{groups[1].str().c_str(), groups[2].str().c_str()};
}

class BoardBasedTest
    : public ::testing::Test
{
public:
    virtual Board createBoard(const char* boardString) = 0;
    virtual void setAsMoved(const char* position) = 0;
};

class WhiteOnMove
    : public BoardBasedTest
{
public:
    virtual Board createBoard(const char* boardString) override
    {
        return utils::createBoard(boardString, color_);
    }

    virtual void setAsMoved(const char* position) {}

    NOTATION::COLOR::color color_ = NOTATION::COLOR::color::white;
};

class BlackOnMove
    : public BoardBasedTest
{
public:
    virtual Board createBoard(const char* boardString) override
    {
        return utils::createBoard(boardString, color_);
    }

    virtual void setAsMoved(const char* position) {}

    NOTATION::COLOR::color color_ = NOTATION::COLOR::color::black;
};

class BlackPawnsInitialMovesTest
    : public testing::WithParamInterface<const char*>
    , public BlackOnMove
{
};

TEST_P(BlackPawnsInitialMovesTest, AAllowToMovePawn)
{
    Board board = createBoard(
            "♜♞♝♛♚♝♞♜"
            "♟♟♟♟♟♟♟♟"
            "        "
            "        "
            "        "
            "        "
            "♙♙♙♙♙♙♙♙"
            "♖♘♗♕♔♗♘♖");
    auto move = createMove(GetParam(), color_);

    ASSERT_TRUE(MoveValidator::validateMove(board, move));
}

INSTANTIATE_TEST_SUITE_P(SinglePawnBlackMoves, BlackPawnsInitialMovesTest, testing::Values(
    "a7-a6", "b7-b6", "c7-c6", "d7-d6", "e7-e6", "f7-f6", "g7-g6", "h7-h6"
    , "a7-a5", "b7-b5", "c7-c5", "d7-d5", "e7-e5", "f7-f5", "g7-g5", "h7-h5"));



class WhitePawnsInitialMovesTest
    : public testing::WithParamInterface<const char*>
    , public WhiteOnMove
{
};

TEST_P(WhitePawnsInitialMovesTest, AllowToMovePawn)
{
	Board board = createBoard(
			"♜♞♝♛♚♝♞♜"
			"♟♟♟♟♟♟♟♟"
			"        "
			"        "
			"        "
			"        "
			"♙♙♙♙♙♙♙♙"
            "♖♘♗♕♔♗♘♖");
    auto move = createMove(GetParam(), color_);

	ASSERT_TRUE(MoveValidator::validateMove(board, move));
}

INSTANTIATE_TEST_SUITE_P(SinglePawnWhiteMoves, WhitePawnsInitialMovesTest, testing::Values(
    "a2-a3", "b2-b3", "c2-c3", "d2-d3", "e2-e3", "f2-f3", "g2-g3", "h2-h3"
    , "a2-a4", "b2-b4", "c2-c4", "d2-d4", "e2-e4", "f2-f4", "g2-g4", "h2-h4"));

TEST_F(WhiteOnMove, DoNotAllowIncorectPawnMoves)
{
    Board board = createBoard(
                "♜♞♝♛♚♝♞♜"
                "♟♟♟ ♟♟♟♟"
                "        "
                "        "
                "        "
                "   ♟    "
                "♙♙♙♙♙♙♙♙"
                "♖♘♗♕♔♗♘♖");

    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("d2-d3", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("d2-d4", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("d2-c3", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("d2-e3", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("d2-c2", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("d2-e2", color_)));
}
