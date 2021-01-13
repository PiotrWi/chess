#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <vector>

#include <detail/MoveApplier.hpp>
#include <detail/MoveGenerator.hpp>
#include <utils/BoardGenerationUtils.hpp>
#include <notations/coordinationnotation.hpp>
#include <notations/LongAlgebraicNotation.hpp>


class MoveGeneratorTests
	: public ::testing::Test
{
public:
	MoveGenerator::MoveGenerator sut;
};

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

std::ostream& operator<<(std::ostream& os, const std::vector<ExtendedMove>& moves)
{
    std::copy(moves.begin(), moves.end(), std::ostream_iterator<ExtendedMove>(os, " "));
    return os;
}

std::vector<ExtendedMove> printAndReturn(const std::vector<ExtendedMove>& moves)
{
    std::cout << moves << std::endl;
    return moves;
}
}  // namespace

TEST_F(MoveGeneratorTests, shouldReturnInitialMoves)
{
    Board board = utils::createBoard(utils::InitialBoardString);
	auto pawnInitialMoves = map(
            {"a2a3", "a2a4", "b2b3", "b2b4", "c2c3", "c2c4", "d2d3",
            "d2d4", "e2e3", "e2e4", "f2f3", "f2f4", "g2g3", "g2g4",
            "h2h3", "h2h4"}, WHITE, board);

	auto knightInitialMoves = map(
            {"Nb1a3", "Nb1c3", "Ng1f3", "Ng1h3"}, WHITE, board);

	ASSERT_THAT(sut.generate(board, WHITE),
		::testing::UnorderedElementsAreArray(pawnInitialMoves + knightInitialMoves));
}

TEST_F(MoveGeneratorTests, shouldCorectlyAnalyzePos_2)
{
	Board board = utils::createBoard(
		"♜♞♝♛♚♝♞♜"
		"♟♟♟   ♟♟"
		"        "
		"   ♟♟♟  "
		"   ♙♙   "
		"♘    ♘  "
		"♙♙♙  ♙♙♙"
		"♖ ♗♕♔♗ ♖");

    auto pawnMoves = map(
            {"b2b3", "b2b4", "c2c3", "c2c4", "d4xe5",
             "e4xd5", "e4xf5", "g2g3", "g2g4",
             "h2h3", "h2h4"}, WHITE, board);

    auto knightMoves = map(
            {"Na3b5", "Na3c4", "Na3b1",
             "Nf3d2", "Nf3xe5", "Nf3g5", "Nf3h4", "Nf3g1"}, WHITE, board);

    auto rockMoves = map(
            {"Ra1b1", "Rh1g1"}, WHITE, board);

    auto bishopMoves = map(
            {"Bc1d2", "Bc1e3", "Bc1f4", "Bc1g5", "Bc1h6",
             "Bf1e2", "Bf1d3", "Bf1c4", "Bf1b5", "Bf1a6"}, WHITE, board);

    auto queenMoves = map(
            {"Qd1d2", "Qd1d3", "Qd1e2"}, WHITE, board);

    auto kingMoves = map(
            {"Ke1d2", "Ke1e2"}, WHITE, board);

	ASSERT_THAT(sut.generate(board, WHITE),
		::testing::UnorderedElementsAreArray(pawnMoves+knightMoves+rockMoves+bishopMoves+queenMoves+kingMoves));
}

TEST_F(MoveGeneratorTests, shouldCorectlyAnalyzePosWithMoves)
{
	Board board = utils::createBoard(
		"    ♚   "
		"  ♟     "
		"        "
		" ♙      "
		"    ♙   "
		"        "
		"    ♔  ♙"
		"♖      ♖");
	auto moveKingToInitialPosition = notations::coordinates::createMove("e2-e1", WHITE);
	MoveApplier::applyMove(board, moveKingToInitialPosition);

    auto moveWitePawnToPieces = notations::coordinates::createMove("c7-c5", BLACK);
    MoveApplier::applyMove(board, moveWitePawnToPieces);

//	"    ♚   "
//	"        "
//	"  .     "
//	" ♙♟     "
//	"    ♙   "
//	"        "
//	"       ♙"
//	"♖   ♔  ♖");

	const char* enPassant = "b5xc6";
	auto pawnMoves = map(
		{enPassant, "b5b6", "e4e5", "h2h3", "h2h4"}, WHITE, board);

	auto rockMoves = map(
		{"Ra1a2", "Ra1a3", "Ra1a4", "Ra1a5", "Ra1a6", "Ra1a7", "Ra1a8",
		"Ra1b1", "Ra1c1", "Ra1d1",
		"Rh1g1", "Rh1f1"}, WHITE, board);

	auto kingMoves = map(
		{"Ke1d1", "Ke1d2", "Ke1e2", "Ke1f2", "Ke1f1"}, WHITE, board);

	ASSERT_THAT(sut.generate(board, WHITE),
		::testing::UnorderedElementsAreArray(pawnMoves+rockMoves+kingMoves));
}

TEST_F(MoveGeneratorTests, shouldAllowCastles)
{
	Board board = utils::createBoard(
		"    ♚   "
		"  ♟     "
		"        "
		" ♙      "
		"    ♙   "
		"        "
		"       ♙"
		"♖   ♔  ♖");

	auto pawnMoves = map(
		{"b5b6", "e4e5", "h2h3", "h2h4"}, WHITE, board);

	auto rockMoves = map(
		{"Ra1a2", "Ra1a3", "Ra1a4", "Ra1a5", "Ra1a6", "Ra1a7", "Ra1a8",
		"Ra1b1", "Ra1c1", "Ra1d1",
		"Rh1g1", "Rh1f1"}, WHITE, board);

	const char* shortCastle = "O-O";
	const char* longCastle = "O-O-O";
	auto kingMoves = map(
		{"Ke1d1", "Ke1d2", "Ke1e2", "Ke1f2", "Ke1f1", shortCastle, longCastle}, WHITE, board);

	ASSERT_THAT(sut.generate(board, WHITE),
		::testing::UnorderedElementsAreArray(pawnMoves+rockMoves+kingMoves));
}

TEST_F(MoveGeneratorTests, shouldPreventIllegalCasles)
{
	Board board = utils::createBoard(
		"   ♜♚ ♜ "
		"  ♟     "
		"        "
		" ♙      "
		"    ♙   "
		"        "
		"       ♙"
		"♖   ♔  ♖");

	auto pawnMoves = map(
		{"b5b6", "e4e5", "h2h3", "h2h4"}, WHITE, board);

	auto rockMoves = map(
		{"Ra1a2", "Ra1a3", "Ra1a4", "Ra1a5", "Ra1a6", "Ra1a7", "Ra1a8",
		"Ra1b1", "Ra1c1", "Ra1d1",
		"Rh1g1", "Rh1f1"}, WHITE, board);

	auto kingMoves = map(
		{"Ke1e2", "Ke1f2", "Ke1f1"}, WHITE, board);

	ASSERT_THAT(sut.generate(board, WHITE),
		::testing::UnorderedElementsAreArray(pawnMoves+rockMoves+kingMoves));
}

TEST_F(MoveGeneratorTests, shouldPreventIllegalCasles_2)
{
	Board board = utils::createBoard(
		"        "
		"  ♟ ♚   "
		"        "
		" ♙      "
		"    ♙   "
		"        "
		"       ♙"
		" ♖  ♔  ♖");

    auto moveRockToInitialPosition = notations::coordinates::createMove("b1-a1", WHITE);
    MoveApplier::applyMove(board, moveRockToInitialPosition);

    auto moveWitePawnToPieces = notations::coordinates::createMove("e7-e8", BLACK);
    MoveApplier::applyMove(board, moveWitePawnToPieces);

	auto pawnMoves = map(
		{"b5b6", "e4e5", "h2h3", "h2h4"}, WHITE, board);

	auto rockMoves = map(
		{"Ra1a2", "Ra1a3", "Ra1a4", "Ra1a5", "Ra1a6", "Ra1a7", "Ra1a8",
		"Ra1b1", "Ra1c1", "Ra1d1",
		"Rh1g1", "Rh1f1"}, WHITE, board);

	const char* shortCastle = "O-O";
	auto kingMoves = map(
		{shortCastle, "Ke1e2", "Ke1f2", "Ke1f1", "Ke1d2", "Ke1d1"}, WHITE, board);


//	"    ♚   "
//	"        "
//	"        "
//	" ♙♟     "
//	"    ♙   "
//	"        "
//	"       ♙"
//	"♖   ♔  ♖");

	ASSERT_THAT(sut.generate(board, WHITE),
		::testing::UnorderedElementsAreArray(pawnMoves+rockMoves+kingMoves));
}

TEST_F(MoveGeneratorTests, shouldPreventIllegalCasles_3)
{
	Board board = utils::createBoard(
		"        "
		"  ♟ ♚   "
		"        "
		" ♙      "
		"    ♙   "
		"        "
		"       ♙"
		"♖   ♔ ♖ ");

    auto moveRockToInitialPosition = notations::coordinates::createMove("g1-h1", WHITE);
    MoveApplier::applyMove(board, moveRockToInitialPosition);

    auto moveWitePawnToPieces = notations::coordinates::createMove("e7-e8", BLACK);
    MoveApplier::applyMove(board, moveWitePawnToPieces);

	auto pawnMoves = map(
		{"b5b6", "e4e5", "h2h3", "h2h4"}, WHITE, board);

	auto rockMoves = map(
		{"Ra1a2", "Ra1a3", "Ra1a4", "Ra1a5", "Ra1a6", "Ra1a7", "Ra1a8",
		"Ra1b1", "Ra1c1", "Ra1d1",
		"Rh1g1", "Rh1f1"}, WHITE, board);

	const char* longCastle = "O-O-O";
	auto kingMoves = map(
		{longCastle, "Ke1e2", "Ke1f2", "Ke1f1", "Ke1d2", "Ke1d1"}, WHITE, board);

//	"    ♚   "
//	"        "
//	"  .     "
//	" ♙♟     "
//	"    ♙   "
//	"        "
//	"       ♙"
//	"♖   ♔  ♖");

	ASSERT_THAT(sut.generate(board, WHITE),
		::testing::UnorderedElementsAreArray(pawnMoves+rockMoves+kingMoves));
}

TEST_F(MoveGeneratorTests, shouldFindPromotions)
{
	Board board = utils::createBoard(
		"  ♞ ♜   "
		"   ♙   ♚"
		"        "
		"        "
		"        "
		"        "
		"        "
		"   ♔    ");
    utils::revokeCastlingRights(board, NOTATION::CASTLING_RIGHTS::WHITE_SHORT_BIT | NOTATION::CASTLING_RIGHTS::WHITE_LONG_BIT);
	auto pawnMoves = map(
		{"d7xc8=Q", "d7xc8=N", "d7xc8=B", "d7xc8=R",
		"d7d8=Q", "d7d8=N", "d7d8=B", "d7d8=R",
		"d7xe8=Q", "d7xe8=N", "d7xe8=B", "d7xe8=R"}, WHITE, board);

	auto kingMoves = map(
		{"Kd1c1", "Kd1c2", "Kd1d2"}, WHITE, board);
	ASSERT_THAT(sut.generate(board, WHITE),
		::testing::UnorderedElementsAreArray(pawnMoves+kingMoves));
}

TEST_F(MoveGeneratorTests, shouldCorectlyAnalyzePosWithCheck)
{
    Board board = utils::createBoard(
            "♜♞♝ ♚♝♞♜"
            "♟♟♟   ♟♟"
            "      ♛ "
            "   ♟♟♟ ♙"
            "   ♙♙  ♖"
            "♘    ♘♔ "
            "♙♙♙  ♙♙ "
            "♖ ♗♕♗   ");

	auto pawnMoves = map(
		{"h5xg6"}, WHITE, board);

	auto knightMoves = map(
		{"Nf3g5"}, WHITE, board);

	auto rockMoves = map(
		{"Rh4g4"}, WHITE, board);

	auto bishopMoves = map(
		{"Bc1g5"}, WHITE, board);

	auto kingMoves = map(
		{"Kg3h3", "Kg3h2"}, WHITE, board);

	ASSERT_THAT(sut.generate(board, WHITE),
		::testing::UnorderedElementsAreArray(pawnMoves+knightMoves+bishopMoves+rockMoves+kingMoves));
}

TEST_F(MoveGeneratorTests, shouldFindInitialMovesForBlack)
{
    Board board = utils::createBoard(utils::InitialBoardString, BLACK);

	auto pawnInitialMoves = map(
			{"a7a6", "a7a5", "b7b6", "b7b5", "c7c6", "c7c5", "d7d6",
			"d7d5", "e7e6", "e7e5", "f7f6", "f7f5", "g7g6", "g7g5",
			"h7h6", "h7h5"}, BLACK, board);

	auto knightInitialMoves = map(
			{"Nb8a6", "Nb8c6", "Ng8f6", "Ng8h6"}, BLACK, board);

	ASSERT_THAT(sut.generate(board, BLACK),
		::testing::UnorderedElementsAreArray(pawnInitialMoves + knightInitialMoves));
}


TEST_F(MoveGeneratorTests, shouldCorectlyAnalyzePosForBlack)
{
    Board board = utils::createBoard(
            "♜ ♝♛♚♝ ♜"
            " ♟♟ ♟♟ ♟"
            "♞     ♟ "
            "♟  ♟    "
            "      ♞ "
            "        "
            "♙♙♙♙♙♙♙♙"
            "♖♘♗♕♔♗♘♖", BLACK);

	auto pawnMoves = map(
		{"a5a4", "b7b6", "b7b5", "c7c6", "c7c5",
		"d5d4", "e7e6", "e7e5", "f7f6", "f7f5",
		"g6g5", "h7h6", "h7h5"}, BLACK, board);
    //
    //
    //   g6g5�U g6g5�U b7b6�U b7b5�U c7c6�U c7c5�U e7e6�U e7e5�U f7f6�U f7f5�U h7h6�U h7h5�U
	auto knightMoves = map(
		{"Na6b8", "Na6c5", "Na6b4",
		"Ng4xh2", "Ng4xf2", "Ng4e3", "Ng4e5", "Ng4f6", "Ng4h6"}, BLACK, board);
	//
    //
	auto rockMoves = map(
		{"Ra8a7", "Ra8b8", "Rh8g8"}, BLACK, board);
    //       Ra8a7    Ra8b8    Rh8g

    auto bishopMoves = map(
		{"Bc8d7", "Bc8e6", "Bc8f5",
		"Bf8g7", "Bf8h6"}, BLACK, board);
    //       Bc8d7    Bc8e6    Bc8f5
    //   Bf8g7    Bf8h6

	auto queenMoves = map(
		{"Qd8d7", "Qd8d6"}, BLACK, board);
        //   Bd8d7    Bd8d6
	auto kingMoves = map(
		{"Ke8d7"}, BLACK,  board);
    //       Ke8d7
//Ng4e5U Ng4f6U Ng4h6U Ng4xh2 Ng4xf2 Ng4e3U a5a4�U d5d4�U Na6b8U Na6c5U Na6b4U
    ASSERT_THAT(printAndReturn(sut.generate(board, BLACK)),
		::testing::UnorderedElementsAreArray(pawnMoves+knightMoves+rockMoves+bishopMoves+queenMoves+kingMoves));
}

TEST_F(MoveGeneratorTests,  shouldCorectlyAnalyzePosForBlack_2)
{
    auto board = utils::createBoard("  ♜   ♚ "
                                    " ♛   ♟♟ "
                                    "    ♟  ♟"
                                    "        "
                                    "♟ ♙  ♖ ♙"
                                    "♙     ♙ "
                                    "  ♗  ♙  "
                                    "     ♕♔ ", BLACK);

    auto pawnMoves = map(
            {"f7f6", "f7f5", "g7g6", "g7g5", "e6e5", "h6h5"}, BLACK, board);

    auto rockMoves = map(
            {"Rc8a8", "Rc8b8", "Rc8d8", "Rc8e8", "Rc8f8",
             "Rc8c7", "Rc8c6", "Rc8c5", "Rc8xc4"}, BLACK, board);

    auto queenMoves = map(
            {"Qb7a8", "Qb7c6", "Qb7d5", "Qb7e4", "Qb7f3", "Qb7g2", "Qb7h1",
             "Qb7a6",
             "Qb7a7", "Qb7c7", "Qb7d7", "Qb7e7",
             "Qb7b8", "Qb7b6", "Qb7b5", "Qb7b4", "Qb7b3", "Qb7b2", "Qb7b1"
            }, BLACK, board);

    auto kingMoves = map(
            {"Kg8f8", "Kg8h8"}, BLACK, board);

    utils::revokeCastlingRights(board, NOTATION::CASTLING_RIGHTS::BLACK_SHORT_BIT | NOTATION::CASTLING_RIGHTS::BLACK_LONG_BIT);
    ASSERT_THAT(sut.generate(board, BLACK),
                ::testing::UnorderedElementsAreArray(pawnMoves+rockMoves+queenMoves+kingMoves));
}

TEST_F(MoveGeneratorTests, shouldFindBlackPromotions)
{
	Board board = utils::createBoard(
		"        "
		"      ♚ "
		"        "
		"        "
		"        "
		"        "
		"     ♟  "
		"   ♔    ");

	auto pawnMoves = map(
		{"f2f1=Q", "f2f1=N", "f2f1=B", "f2f1=R"}, BLACK, board);

	auto kingMoves = map(
		{"Kg7g8", "Kg7h8", "Kg7h7", "Kg7h6",
		"Kg7g6", "Kg7f6", "Kg7f7", "Kg7f8"}, BLACK, board);

	ASSERT_THAT(sut.generate(board, BLACK),
		::testing::UnorderedElementsAreArray(pawnMoves+kingMoves));
}

TEST_F(MoveGeneratorTests, shallNotAllowToMovePinned)
{
    auto board = utils::createBoard("    ♜ ♚ "
                                    "        "
                                    "    ♖   "
                                    "♛  ♖♔♖♜ "
                                    "    ♖   "
                                    "        "
                                    "    ♛   "
                                    "        ", WHITE);

    auto rockMoves = map(
            {"Re6e7", "Re6xe8",
             "Re4e3", "Re4xe2",
             "Rd5c5", "Rd5b5", "Rd5xa5", "Rf5xg5"}, WHITE, board);
            //   Re6e7V,  Re6xe8
            //   Re4e3V, Re4xe2
            //   Rd5c5V, Rd5b5V, Rd5xa5
    auto kingMoves = map(
            {"Ke5f6", "Ke5d6", "Ke5d4", "Ke5f4"}, WHITE, board);
//  Actual: { , , Rf5f4V, Rf5f3V, Rf5f2V, Rf5f1V, Rf5f6V, Rf5f7V, Rf5f8V, Rf5xg5, Ke5f6V, Ke5f4V, Ke5d4V, Ke5d6V }, which has 19 elements
    ASSERT_THAT(sut.generate(board, WHITE),
                ::testing::UnorderedElementsAreArray(rockMoves + kingMoves));
}

TEST_F(MoveGeneratorTests, shallNotAllowToMovePinnedBlack)
{
    auto board = utils::createBoard("    ♖ ♔ "
                                    "        "
                                    "    ♜   "
                                    "♖  ♜♚♜♖ "
                                    "    ♜   "
                                    "        "
                                    "    ♖   "
                                    "        ", BLACK);
    auto rockMoves = map(
            {"Re6e7", "Re6xe8",
             "Re4e3", "Re4xe2",
             "Rd5c5", "Rd5b5", "Rd5xa5", "Rf5xg5"}, BLACK, board);


    auto kingMoves = map(
            {"Ke5f6", "Ke5d6", "Ke5d4", "Ke5f4"}, BLACK, board);


    ASSERT_THAT(sut.generate(board, BLACK),
                ::testing::UnorderedElementsAreArray(rockMoves + kingMoves));
}

TEST_F(MoveGeneratorTests, shallNotAllowToMoveDiagonal)
{
    auto board = utils::createBoard("      ♚♛"
                                    "  ♛     "
                                    "   ♖ ♖  "
                                    "    ♔   "
                                    "   ♖ ♖  "
                                    "      ♛ "
                                    " ♛      "
                                    "        ", WHITE);
    auto kingMoves = map(
            {"Ke5e6", "Ke5d5", "Ke5f5", "Ke5e4"}, WHITE, board);

    ASSERT_THAT(sut.generate(board, WHITE),
                ::testing::UnorderedElementsAreArray(kingMoves));
}

TEST_F(MoveGeneratorTests, shallNotAllowToMoveDiagonalBlack)
{
    auto board = utils::createBoard("♔       "
                                    "  ♕   ♕ "
                                    "   ♜ ♜  "
                                    "    ♚   "
                                    "   ♜ ♜  "
                                    "  ♕   ♕ "
                                    "        "
                                    "        ", BLACK);


    auto kingMoves = map(
            {"Ke5e6", "Ke5d5", "Ke5f5", "Ke5e4"}, BLACK, board);

    ASSERT_THAT(sut.generate(board, BLACK),
                ::testing::UnorderedElementsAreArray(kingMoves));
}

TEST_F(MoveGeneratorTests, shallNotAllowToMovePawnWhenPinned)
{
    auto board = utils::createBoard(
            "  ♔ ♚   "
            "     ♟  "
            "        "
            "       ♕"
            "        "
            "        "
            "        "
            "        ", BLACK);

    auto kingMoves = map(
            {"Ke8f8", "Ke8e7"}, BLACK, board);

    ASSERT_THAT(sut.generate(board, BLACK),
                ::testing::UnorderedElementsAreArray(kingMoves));
}



// CUSTOM from BUG fixing
TEST_F(MoveGeneratorTests, Custom_1)
{
    auto board = utils::createBoard("♜♞♝ ♚ ♞♜"
        "♟♟♟  ♟♟♟"
        "    ♟   "
        "   ♛    "
        "   ♙    "
        "♙ ♝     "
        " ♙♙  ♙♙♙"
        "♖ ♗♕♔♗♘♖");

    auto kingMoves = map(
            {"Ke1e2"}, WHITE, board);
    auto pawnMoves = map(
            {"b2xc3"}, WHITE, board);
    auto queenMoves = map(
            {"Qd1d2"}, WHITE, board);
    auto bishopMoves = map(
            {"Bc1d2"}, WHITE, board);
    ASSERT_THAT(sut.generate(board, WHITE),
                ::testing::UnorderedElementsAreArray(kingMoves + pawnMoves + queenMoves + bishopMoves));
}
