#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <vector>

#include <detail/MoveApplier.hpp>
#include <detail/MoveGenerator.hpp>
#include <utils/BoardGenerationUtils.hpp>
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

TEST_F(MoveGeneratorTests, shouldReturnInitialMoves)
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

	auto moveKingToInitialPosition = createMove("e2-e1", WHITE);
	MoveApplier::applyMove(board, moveKingToInitialPosition);

	auto moveWitePawnToPieces = createMove("c7-c5", BLACK);
    MoveApplier::applyMove(board, moveWitePawnToPieces);

//	"    ♚   "
//	"        "
//	"  .     "
//	" ♙♟     "
//	"    ♙   "
//	"        "
//	"       ♙"
//	"♖   ♔  ♖");

	const char* enPassant = "b5-c6";
	auto pawnMoves = map(
		{enPassant, "b5-b6", "e4-e5", "h2-h3", "h2-h4"}, WHITE);

	auto rockMoves = map(
		{"a1-a2", "a1-a3", "a1-a4", "a1-a5", "a1-a6", "a1-a7", "a1-a8",
		"a1-b1", "a1-c1", "a1-d1",
		"h1-g1", "h1-f1"}, WHITE);

	auto kingMoves = map(
		{"e1-d1", "e1-d2", "e1-e2", "e1-f2", "e1-f1"}, WHITE);

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
		{"b5-b6", "e4-e5", "h2-h3", "h2-h4"}, WHITE);

	auto rockMoves = map(
		{"a1-a2", "a1-a3", "a1-a4", "a1-a5", "a1-a6", "a1-a7", "a1-a8",
		"a1-b1", "a1-c1", "a1-d1",
		"h1-g1", "h1-f1"}, WHITE);

	const char* shortCastle = "O-O";
	const char* longCastle = "O-O-O";
	auto kingMoves = map(
		{"e1-d1", "e1-d2", "e1-e2", "e1-f2", "e1-f1", shortCastle, longCastle}, WHITE);

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
		{"b5-b6", "e4-e5", "h2-h3", "h2-h4"}, WHITE);

	auto rockMoves = map(
		{"a1-a2", "a1-a3", "a1-a4", "a1-a5", "a1-a6", "a1-a7", "a1-a8",
		"a1-b1", "a1-c1", "a1-d1",
		"h1-g1", "h1-f1"}, WHITE);

	auto kingMoves = map(
		{"e1-e2", "e1-f2", "e1-f1"}, WHITE);

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

	auto pawnMoves = map(
		{"b5-b6", "e4-e5", "h2-h3", "h2-h4"}, WHITE);

	auto rockMoves = map(
		{"a1-a2", "a1-a3", "a1-a4", "a1-a5", "a1-a6", "a1-a7", "a1-a8",
		"a1-b1", "a1-c1", "a1-d1",
		"h1-g1", "h1-f1"}, WHITE);

	const char* shortCastle = "O-O";
	auto kingMoves = map(
		{shortCastle, "e1-e2", "e1-f2", "e1-f1", "e1-d2", "e1-d1"}, WHITE);

	auto moveKingToInitialPosition = createMove("b1-a1", WHITE);
    MoveApplier::applyMove(board, moveKingToInitialPosition);

	auto moveWitePawnToPieces = createMove("e7-e8", BLACK);
    MoveApplier::applyMove(board, moveWitePawnToPieces);

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

	auto pawnMoves = map(
		{"b5-b6", "e4-e5", "h2-h3", "h2-h4"}, WHITE);

	auto rockMoves = map(
		{"a1-a2", "a1-a3", "a1-a4", "a1-a5", "a1-a6", "a1-a7", "a1-a8",
		"a1-b1", "a1-c1", "a1-d1",
		"h1-g1", "h1-f1"}, WHITE);

	const char* longCastle = "O-O-O";
	auto kingMoves = map(
		{longCastle, "e1-e2", "e1-f2", "e1-f1", "e1-d2", "e1-d1"}, WHITE);

	auto moveKingToInitialPosition = createMove("g1-h1", WHITE);
    MoveApplier::applyMove(board, moveKingToInitialPosition);

	auto moveWitePawnToPieces = createMove("e7-e8", BLACK);
    MoveApplier::applyMove(board, moveWitePawnToPieces);

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

	auto pawnMoves = map(
		{"d7-c8=Q", "d7-c8=N", "d7-c8=B", "d7-c8=R",
		"d7-d8=Q", "d7-d8=N", "d7-d8=B", "d7-d8=R",
		"d7-e8=Q", "d7-e8=N", "d7-e8=B", "d7-e8=R"}, WHITE);

	auto kingMoves = map(
		{"d1-c1", "d1-c2", "d1-d2"}, WHITE);

	ASSERT_THAT(sut.generate(board, WHITE),
		::testing::UnorderedElementsAreArray(pawnMoves+kingMoves));
}

TEST_F(MoveGeneratorTests, shouldCorectlyAnalyzePosWithCheck)
{
	auto pawnMoves = map(
		{"h5-g6"}, WHITE);

	auto knightMoves = map(
		{"f3-g5"}, WHITE);

	auto rockMoves = map(
		{"h4-g4"}, WHITE);

	auto bishopMoves = map(
		{"c1-g5"}, WHITE);

	auto kingMoves = map(
		{"g3-h3", "g3-h2"}, WHITE);

	Board board = utils::createBoard(
		"♜♞♝ ♚♝♞♜"
		"♟♟♟   ♟♟"
		"      ♛ "
		"   ♟♟♟ ♙"
		"   ♙♙  ♖"
		"♘    ♘♔ "
		"♙♙♙  ♙♙ "
		"♖ ♗♕♗   ");

	ASSERT_THAT(sut.generate(board, WHITE),
		::testing::UnorderedElementsAreArray(pawnMoves+knightMoves+bishopMoves+rockMoves+kingMoves));
}


TEST_F(MoveGeneratorTests, shouldFindInitialMovesForBlack)
{
	auto pawnInitialMoves = map(
			{"a7-a6", "a7-a5", "b7-b6", "b7-b5", "c7-c6", "c7-c5", "d7-d6",
			"d7-d5", "e7-e6", "e7-e5", "f7-f6", "f7-f5", "g7-g6", "g7-g5",
			"h7-h6", "h7-h5"}, BLACK);

	auto knightInitialMoves = map(
			{"b8-a6", "b8-c6", "g8-f6", "g8-h6"}, BLACK);

	Board board = utils::createBoard(utils::InitialBoardString);
	ASSERT_THAT(sut.generate(board, BLACK),
		::testing::UnorderedElementsAreArray(pawnInitialMoves + knightInitialMoves));
}


TEST_F(MoveGeneratorTests, shouldCorectlyAnalyzePosForBlack)
{
	auto pawnMoves = map(
		{"a5-a4", "b7-b6", "b7-b5", "c7-c6", "c7-c5",
		"d5-d4", "e7-e6", "e7-e5", "f7-f6", "f7-f5",
		"g6-g5", "h7-h6", "h7-h5"}, BLACK);

	auto knightMoves = map(
		{"a6-b8", "a6-c5", "a6-b4",
		"g4-h2", "g4-f2", "g4-e3", "g4-e5", "g4-f6", "g4-h6"}, BLACK);

	auto rockMoves = map(
		{"a8-a7", "a8-b8", "h8-g8"}, BLACK);

	auto bishopMoves = map(
		{"c8-d7", "c8-e6", "c8-f5",
		"f8-g7", "f8-h6"}, BLACK);

	auto queenMoves = map(
		{"d8-d7", "d8-d6"}, BLACK);

	auto kingMoves = map(
		{"e8-d7"}, BLACK);

	Board board = utils::createBoard(
		"♜ ♝♛♚♝ ♜"
		" ♟♟ ♟♟ ♟"
		"♞     ♟ "
		"♟  ♟    "
		"      ♞ "
		"        "
		"♙♙♙♙♙♙♙♙"
		"♖♘♗♕♔♗♘♖", BLACK);

	ASSERT_THAT(sut.generate(board, BLACK),
		::testing::UnorderedElementsAreArray(pawnMoves+knightMoves+rockMoves+bishopMoves+queenMoves+kingMoves));
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
		{"f2-f1=Q", "f2-f1=N", "f2-f1=B", "f2-f1=R"}, BLACK);

	auto kingMoves = map(
		{"g7-g8", "g7-h8", "g7-h7", "g7-h6",
		"g7-g6", "g7-f6", "g7-f7", "g7-f8"}, BLACK);

	ASSERT_THAT(sut.generate(board, BLACK),
		::testing::UnorderedElementsAreArray(pawnMoves+kingMoves));
}
