#include <gtest/gtest.h>

#include <publicIf/Board.hpp>
#include <detail/MoveValidator.hpp>

#include <utils/BoardGenerationUtils.hpp>
#include <utils/LongAlgebraicNotation.hpp>

class BoardBasedTest
    : public ::testing::Test
{
public:
    virtual Board createBoard(const char* boardString) const = 0;
    void setAsMoved(Board& board, const char* fieldStr) const
    {
    	board[fieldStr] |= NOTATION::MOVED::MOVED_MASK;
    }
};

class WhiteOnMove
    : public BoardBasedTest
{
public:
    virtual Board createBoard(const char* boardString) const override
    {
        return utils::createBoard(boardString, color_);
    }
    NOTATION::COLOR::color color_ = NOTATION::COLOR::color::white;
};

class BlackOnMove
    : public BoardBasedTest
{
public:
    virtual Board createBoard(const char* boardString) const override
    {
        return utils::createBoard(boardString, color_);
    }
    NOTATION::COLOR::color color_ = NOTATION::COLOR::color::black;
};

/*******************************************************************
 * PAWNs tests
 ******************************************************************/

class MoveValidatorTests_BlackPawnsInitialMovesTest
    : public testing::WithParamInterface<const char*>
    , public BlackOnMove
{
};

TEST_P(MoveValidatorTests_BlackPawnsInitialMovesTest, AAllowToMovePawn)
{
    Board board = createBoard(utils::InitialBoardString);
    auto move = createMove(GetParam(), color_);

    ASSERT_TRUE(MoveValidator::validateMove(board, move));
}

INSTANTIATE_TEST_SUITE_P(MoveValidatorTests_SinglePawnBlackMoves, MoveValidatorTests_BlackPawnsInitialMovesTest, testing::Values(
    "a7-a6", "b7-b6", "c7-c6", "d7-d6", "e7-e6", "f7-f6", "g7-g6", "h7-h6"
    , "a7-a5", "b7-b5", "c7-c5", "d7-d5", "e7-e5", "f7-f5", "g7-g5", "h7-h5"));



class MoveValidatorTests_WhitePawnsInitialMovesTest
    : public testing::WithParamInterface<const char*>
    , public WhiteOnMove
{
};

TEST_P(MoveValidatorTests_WhitePawnsInitialMovesTest, AllowToMovePawn)
{
	Board board = createBoard(utils::InitialBoardString);
    auto move = createMove(GetParam(), color_);

	ASSERT_TRUE(MoveValidator::validateMove(board, move));
}

INSTANTIATE_TEST_SUITE_P(MoveValidatorTests_SinglePawnWhiteMoves, MoveValidatorTests_WhitePawnsInitialMovesTest, testing::Values(
    "a2-a3", "b2-b3", "c2-c3", "d2-d3", "e2-e3", "f2-f3", "g2-g3", "h2-h3"
    , "a2-a4", "b2-b4", "c2-c4", "d2-d4", "e2-e4", "f2-f4", "g2-g4", "h2-h4"));

class MoveValidatorTests_WhitePawnOnMove : public WhiteOnMove {};

TEST_F(MoveValidatorTests_WhitePawnOnMove, DoNotAllowIncorectPawnMoves)
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

TEST_F(MoveValidatorTests_WhitePawnOnMove, AllowStandardBeats)
{
    Board board = createBoard(
                "♜♞♝♛♚♝♞♜"
                "♟♟♟   ♟♟"
                "        "
                "   ♟♟♟  "
                "    ♙   "
                "        "
                "♙♙♙♙ ♙♙♙"
                "♖♘♗♕♔♗♘♖");

    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e4-d5", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e4-f5", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("e4-e5", color_)));
}

TEST_F(MoveValidatorTests_WhitePawnOnMove, AllowBeatsOnFly)
{
    Board board = createBoard(
                "♜♞♝♛♚♝♞♜"
                "♟♟♟   ♟♟"
                "    ♟   "
                "   ♟♙♟  "
                "        "
                "        "
                "♙♙♙♙ ♙♙♙"
                "♖♘♗♕♔♗♘♖");

    board.lastMove = createMove("d7-d5", NOTATION::COLOR::color::black);
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e5-d6", color_)));

    board.lastMove = createMove("f7-f5", NOTATION::COLOR::color::black);
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e5-f6", color_)));

    board.lastMove = createMove("e7-e6", NOTATION::COLOR::color::black);
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("e5-e6", color_)));
}

TEST_F(MoveValidatorTests_WhitePawnOnMove, DoNotAllowToCheckAfterMove)
{
    Board board = createBoard(
                "    ♚   "
                " ♛  ♛   "
                "     ♛  "
                "   ♙♙   "
                "    ♔♙ ♛"
                "        "
                "        "
                "        ");

    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("e5-g6", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("f4-f5", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("d5-d6", color_)));
}


class MoveValidatorTests_BlackPawnOnMove : public BlackOnMove {};

TEST_F(MoveValidatorTests_BlackPawnOnMove, DoNotAllowIncorectPawnMoves)
{
    Board board = createBoard(
                "♜♞♝♛♚♝♞♜"
                "♟♟♟♟♟♟♟♟"
                "   ♙    "
                "        "
                "        "
                "        "
                "♙♙♙ ♙♙♙♙"
                "♖♘♗♕♔♗♘♖");

    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("d7-d6", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("d7-d5", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("d7-c6", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("d7-e6", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("d7-e7", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("d7-c7", color_)));
}

TEST_F(MoveValidatorTests_BlackPawnOnMove, AllowStandardBeats)
{
    Board board = createBoard(
                "♜♞♝♛♚♝♞♜"
                "♟♟♟♟ ♟♟♟"
                "        "
                "    ♟   "
                "   ♙♙♙  "
                "        "
                "♙♙♙  ♙♙♙"
                "♖♘♗♕♔♗♘♖");

    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e5-d4", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e5-f4", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("e5-e4", color_)));
}

TEST_F(MoveValidatorTests_BlackPawnOnMove, AllowBeatsOnFly)
{
    Board board = createBoard(
                "♜♞♝♛♚♝♞♜"
                "♟♟♟♟ ♟♟♟"
                "        "
                "        "
                "   ♙♟♙  "
                "    ♙   "
                "♙♙♙   ♙♙"
                "♖♘♗♕♔♗♘♖");

    board.lastMove = createMove("d2-d4", NOTATION::COLOR::color::black);
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e4-d3", color_)));

    board.lastMove = createMove("f2-f4", NOTATION::COLOR::color::black);
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e4-f3", color_)));

    board.lastMove = createMove("e7-e6", NOTATION::COLOR::color::black);
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("e4-e3", color_)));
}

TEST_F(MoveValidatorTests_BlackPawnOnMove, DoNotAllowToCheckAfterMove)
{
    Board board = createBoard(
                "        "
                "    ♚♟ ♕"
                "   ♟♟   "
                "     ♕  "
                "        "
                "♕   ♕   "
                "        "
                "    ♔   ");

    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("e6-g5", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("g7-g6", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("d6-d5", color_)));
}

/*******************************************************************
 * ROCKs tests
 ******************************************************************/

class MoveValidatorTests_WhiteRockOnMove : public WhiteOnMove {};

TEST_F(MoveValidatorTests_WhiteRockOnMove, DoNotAllowIncorectPawnMoves)
{
    Board board = createBoard(utils::InitialBoardString);

    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("a1-a3", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("a1-c1", color_)));
}

class MoveValidatorTests_WhiteRockAllMovies
	: public MoveValidatorTests_WhiteRockOnMove
	, public ::testing::WithParamInterface<const char*>
{
};

TEST_P(MoveValidatorTests_WhiteRockAllMovies, AllowRockMovies)
{
    Board board = createBoard(
                "    ♚   "
                "        "
                "        "
                "        "
                "        "
                "  ♖     "
                "        "
                "    ♔   ");

    ASSERT_TRUE(MoveValidator::validateMove(board, createMove(GetParam(), color_)));
}

INSTANTIATE_TEST_SUITE_P(MoveValidatorTests_WhiteRockAllMovies, MoveValidatorTests_WhiteRockAllMovies, testing::Values(
    "c3-c1", "c3-c2", "c3-c4", "c3-c5", "c3-c6", "c3-c7", "c3-c8"
    , "c3-a3", "c3-b3", "c3-d3", "c3-e3", "c3-f3", "c3-g3", "c3-h3"));

TEST_F(MoveValidatorTests_WhiteRockOnMove, DoNotAllowIncorectMoves)
{
    Board board = createBoard(
                "♜   ♚   "
                "♟       "
                "        "
                "        "
                "        "
                "        "
                "        "
                "♖  ♔    ");

    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("a1-d1", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("a1-e1", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("a1-b2", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("a1-a8", color_)));
}

TEST_F(MoveValidatorTests_WhiteRockOnMove, AllowBeats)
{
    Board board = createBoard(
                "♜   ♚   "
                "♟       "
                "        "
                "        "
                "        "
                "        "
                "   ♔    "
                "♖ ♜     ");

    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("a1-c1", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("a1-a7", color_)));
}


class MoveValidatorTests_BlackRockOnMove : public BlackOnMove {};

TEST_F(MoveValidatorTests_BlackRockOnMove, DoNotAllowIncorectMoves)
{
    Board board = createBoard(utils::InitialBoardString);

    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("a8-a7", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("a8-c8", color_)));
}

class MoveValidatorTests_BlackRockAllMovies
	: public MoveValidatorTests_BlackRockOnMove
	, public ::testing::WithParamInterface<const char*>
{
};

TEST_P(MoveValidatorTests_BlackRockAllMovies, AllowRockMovies)
{
    Board board = createBoard(
                "    ♚   "
                "        "
                "        "
                "        "
                "        "
                "  ♜     "
                "        "
                "    ♔   ");

    ASSERT_TRUE(MoveValidator::validateMove(board, createMove(GetParam(), color_)));
}

INSTANTIATE_TEST_SUITE_P(MoveValidatorTests_BlackRockAllMovies, MoveValidatorTests_BlackRockAllMovies, testing::Values(
    "c3-c1", "c3-c2", "c3-c4", "c3-c5", "c3-c6", "c3-c7", "c3-c8"
    , "c3-a3", "c3-b3", "c3-d3", "c3-e3", "c3-f3", "c3-g3", "c3-h3"));

TEST_F(MoveValidatorTests_BlackRockOnMove, DoNotAllowIncorectMoves2)
{
    Board board = createBoard(
                "♜   ♚   "
                "        "
                "        "
                "        "
                "        "
                "        "
                "♟       "
                "♖  ♔    ");

    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("a8-e8", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("a8-a2", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("a8-a1", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("a8-h8", color_)));
}

TEST_F(MoveValidatorTests_BlackRockOnMove, AllowBeats)
{
    Board board = createBoard(
                "♜   ♖   "
                "      ♚ "
                "        "
                "        "
                "        "
                "        "
                "♖       "
                "   ♔    ");

    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("a8-e8", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("a8-a2", color_)));
}

/*******************************************************************
 * KNIGHTs tests
 ******************************************************************/

class MoveValidatorTests_WhiteKnightOnMove : public WhiteOnMove {};

TEST_F(MoveValidatorTests_WhiteKnightOnMove, CheckInitialMoves)
{
    Board board = createBoard(utils::InitialBoardString);

    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("b1-a3", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("b1-c3", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("b1-d2", color_)));


    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("g1-f3", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("g1-h3", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("g1-e2", color_)));
}

TEST_F(MoveValidatorTests_WhiteKnightOnMove, CheckAllMoves)
{
    Board board = createBoard(
                "    ♚   "
                "        "
                "        "
                "        "
                "  ♘     "
                "        "
                "        "
                "    ♔   ");

    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("c4-a3", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("c4-a5", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("c4-b2", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("c4-b6", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("c4-d2", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("c4-d6", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("c4-e3", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("c4-e5", color_)));

    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("c4-d4", color_)));
}


class MoveValidatorTests_BlackKnightOnMove : public BlackOnMove {};

TEST_F(MoveValidatorTests_BlackKnightOnMove, CheckInitialMoves)
{
    Board board = createBoard(utils::InitialBoardString);

    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("b8-a6", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("b8-c6", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("b8-d7", color_)));


    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("g8-f6", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("g8-h6", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("g8-e7", color_)));
}

TEST_F(MoveValidatorTests_BlackKnightOnMove, CheckAllMoves)
{
    Board board = createBoard(
                "    ♚   "
                "        "
                "        "
                "        "
                "  ♞     "
                "        "
                "        "
                "    ♔   ");

    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("c4-a3", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("c4-a5", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("c4-b2", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("c4-b6", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("c4-d2", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("c4-d6", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("c4-e3", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("c4-e5", color_)));

    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("c4-d4", color_)));
}

/*******************************************************************
 * BISHOPs tests
 ******************************************************************/

class MoveValidatorTests_WhiteBishopOnMove : public WhiteOnMove {};

TEST_F(MoveValidatorTests_WhiteBishopOnMove, CheckInitialMoves)
{
    Board board = createBoard(utils::InitialBoardString);

    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("c1-b2", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("c1-a3", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("c1-d2", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("c1-e2", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("f1-e2", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("f1-g3", color_)));
}

TEST_F(MoveValidatorTests_WhiteBishopOnMove, CheckAllMoves)
{
    Board board = createBoard(
    		"    ♚   "
            "        "
            "        "
            "    ♝   "
            "    ♗   "
            "        "
            "        "
            "    ♔   ");

    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e4-d3", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e4-c2", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e4-b1", color_)));

    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e4-f3", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e4-g2", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e4-h1", color_)));

    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e4-f5", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e4-g6", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e4-h7", color_)));

    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e4-d5", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e4-c6", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e4-b7", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e4-a8", color_)));

    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("e4-e5", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("e4-f4", color_)));
}


TEST_F(MoveValidatorTests_WhiteBishopOnMove, StandardBeats)
{
    Board board = createBoard(
    		"    ♚   "
            " ♜     ♜"
            "  ♜   ♜ "
            "    ♝   "
            "    ♗   "
            "        "
            "  ♜   ♜ "
            "    ♔   ");

    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e4-g6", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("e4-h7", color_)));

    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e4-c6", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("e4-b7", color_)));

    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e4-c2", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("e4-b1", color_)));

    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e4-g2", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("e4-h1", color_)));
}

class MoveValidatorTests_BlackBishopOnMove : public BlackOnMove {};

TEST_F(MoveValidatorTests_BlackBishopOnMove, CheckInitialMoves)
{
    Board board = createBoard(utils::InitialBoardString);

    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("c8-b7", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("c8-a6", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("c8-d7", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("c8-e7", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("f8-e7", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("f8-g6", color_)));
}

TEST_F(MoveValidatorTests_BlackBishopOnMove, CheckAllMoves)
{
    Board board = createBoard(
    		"    ♚   "
            "        "
            "        "
            "    ♝   "
            "    ♗   "
            "        "
            "        "
            "    ♔   ");

    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e5-d4", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e5-c3", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e5-b2", color_)));

    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e5-f4", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e5-g3", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e5-h2", color_)));

    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e5-f6", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e5-g7", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e5-h8", color_)));

    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e5-d6", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e5-c7", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e5-b8", color_)));

    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("e5-e5", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("e5-f5", color_)));
}


TEST_F(MoveValidatorTests_BlackBishopOnMove, StandardBeats)
{
    Board board = createBoard(
    		"    ♚   "
            "  ♖   ♖ "
            "        "
            "    ♝   "
            "    ♗   "
            "  ♖   ♖ "
            " ♖     ♖"
            "    ♔   ");

    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e5-g7", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("e5-h8", color_)));

    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e5-c7", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("e5-b8", color_)));

    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e5-c3", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("e5-b2", color_)));

    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e5-g3", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("e5-h2", color_)));
}

/*******************************************************************
 * QUEENSs tests
 ******************************************************************/

class MoveValidatorTests_WhiteQueenOnMove : public WhiteOnMove {};

TEST_F(MoveValidatorTests_WhiteQueenOnMove, CheckInitialMoves)
{
    Board board = createBoard(utils::InitialBoardString);

    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("d1-c1", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("d1-c2", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("d1-d2", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("d1-e2", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("d1-e1", color_)));
}

TEST_F(MoveValidatorTests_WhiteQueenOnMove, CheckAllMoves)
{
    Board board = createBoard(
    		"    ♚   "
            "        "
            "        "
            "        "
            "  ♕     "
            "        "
            "        "
            "    ♔   "); //c4

    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("c4-a4", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("c4-a6", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("c4-c8", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("c4-g8", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("c4-c8", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("c4-f1", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("c4-c1", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("c4-a2", color_)));
}

class MoveValidatorTests_BlackQueenOnMove : public BlackOnMove {};

TEST_F(MoveValidatorTests_BlackQueenOnMove, CheckInitialMoves)
{
    Board board = createBoard(utils::InitialBoardString);

    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("d8-c8", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("d8-c7", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("d8-d7", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("d8-e7", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("d8-e8", color_)));
}

TEST_F(MoveValidatorTests_BlackQueenOnMove, CheckAllMoves)
{
    Board board = createBoard(
    		"    ♚   "
            "        "
            "        "
            "        "
            "  ♛     "
            "        "
            "        "
            "    ♔   "); //c4

    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("c4-a4", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("c4-a6", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("c4-c8", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("c4-g8", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("c4-c8", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("c4-f1", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("c4-c1", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("c4-a2", color_)));
}

/*******************************************************************
 * KINGs tests
 ******************************************************************/

class MoveValidatorTests_WhiteKingOnMove : public WhiteOnMove {};

TEST_F(MoveValidatorTests_WhiteKingOnMove, CheckInitialMoves)
{
    Board board = createBoard(utils::InitialBoardString);

    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("e1-d1", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("e1-d2", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("e1-e2", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("e1-f2", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("e1-f1", color_)));
}

TEST_F(MoveValidatorTests_WhiteKingOnMove, CheckAllMoves)
{
    Board board = createBoard(
    		"    ♚   "
            "        "
            "        "
            "        "
            "  ♕     "
            "        "
            "    ♔   "
            "        "); //c4

    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e2-e1", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e2-f1", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e2-f2", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e2-f3", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e2-e3", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e2-d3", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e2-d2", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("e2-d1", color_)));
}

TEST_F(MoveValidatorTests_WhiteKingOnMove, CheckCastles)
{
    Board board = createBoard(
    		"    ♚   "
            "        "
            "        "
            "        "
            "  ♕     "
            "        "
            "        "
            "♖   ♔  ♖");

    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("O-O", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("O-O-O", color_)));
}

TEST_F(MoveValidatorTests_WhiteKingOnMove, DoNotAllowCastleWhenMoved)
{
	const char* boardString =
    		"    ♚   "
            "        "
            "        "
            "        "
            "  ♕     "
            "        "
            "        "
            "♖   ♔  ♖";
    Board board = createBoard(boardString);
    setAsMoved(board, "a1");
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("O-O", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("O-O-O", color_)));

    board = createBoard(boardString);
    setAsMoved(board, "h1");
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("O-O", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("O-O-O", color_)));

    board = createBoard(boardString);
    setAsMoved(board, "e1");
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("O-O", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("O-O-O", color_)));
}


TEST_F(MoveValidatorTests_WhiteKingOnMove, DoNotAllowCastlesOverAtackedFields)
{
    Board board = createBoard(
    		"   ♜♚♜  "
            "        "
            "        "
            "        "
            "  ♕     "
            "        "
            "        "
            "♖   ♔  ♖");

    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("O-O", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("O-O-O", color_)));
}


class MoveValidatorTests_BlackKingOnMove : public BlackOnMove {};

TEST_F(MoveValidatorTests_BlackKingOnMove, CheckInitialMoves)
{
    Board board = createBoard(utils::InitialBoardString);

    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("e8-d8", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("e8-d7", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("e8-e7", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("e8-f7", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("e8-f8", color_)));
}

TEST_F(MoveValidatorTests_BlackKingOnMove, CheckAllMoves)
{
    Board board = createBoard(
    		"        "
            " ♚      "
            "        "
            "        "
            "  ♛     "
            "        "
            "        "
            "    ♔   "); //c4

    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("b7-a8", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("b7-b8", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("b7-c8", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("b7-c7", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("b7-c6", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("b7-b6", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("b7-a6", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("b7-a7", color_)));
}


TEST_F(MoveValidatorTests_BlackKingOnMove, CheckCastles)
{
    Board board = createBoard(
    		"♜   ♚  ♜"
            "        "
            "        "
            "        "
            "        "
            "        "
            "        "
            "    ♔   ");

    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("O-O", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("O-O-O", color_)));
}

TEST_F(MoveValidatorTests_BlackKingOnMove, DoNotAllowCastleWhenMoved)
{
	const char* boardString =
    		"♜   ♚  ♜"
            "        "
            "        "
            "        "
            "        "
            "        "
            "        "
            "    ♔   ";
    Board board = createBoard(boardString);
    setAsMoved(board, "a8");
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("O-O", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("O-O-O", color_)));

    board = createBoard(boardString);
    setAsMoved(board, "h8");
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("O-O", color_)));
    ASSERT_TRUE(MoveValidator::validateMove(board, createMove("O-O-O", color_)));

    board = createBoard(boardString);
    setAsMoved(board, "e8");
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("O-O", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("O-O-O", color_)));
}


TEST_F(MoveValidatorTests_BlackKingOnMove, DoNotAllowCastlesOverAtackedFields)
{
    Board board = createBoard(
    		"♜   ♚  ♜"
            "        "
            "        "
            "        "
            "        "
            "        "
            "        "
            "   ♖♔♖  ");

    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("O-O", color_)));
    ASSERT_FALSE(MoveValidator::validateMove(board, createMove("O-O-O", color_)));
}
