#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <vector>

#include <detail/MoveApplier.hpp>
#include <MoveGeneratorV2/MoveGeneratorV2.hpp>
#include <utils/BoardGenerationUtils.hpp>
#include <notations/coordinationnotation.hpp>
#include <notations/LongAlgebraicNotation.hpp>

bool operator==(const std::span<ExtendedMove>& lhs, const std::vector<ExtendedMove>& rhs)
{
    if (lhs.size() != rhs.size())
    {
        return false;
    }
    for (unsigned int i = 0; i < lhs.size(); ++i)
    {
        if (lhs[i] != rhs[i])
        {
            return false;
        }
    }
    return true;
}

bool operator==(const std::span<ExtendedMove>& lhs, const std::span<ExtendedMove>& rhs)
{
    if (lhs.size() != rhs.size())
    {
        return false;
    }
    for (unsigned int i = 0; i < lhs.size(); ++i)
    {
        if (lhs[i] != rhs[i])
        {
            return false;
        }
    }
    return true;
}

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


}  // namespace

TEST(MoveGeneratorTestsV2, shouldCorrectlyHandleJustNotAttackedKingMoves)
{
    {
        Board board = utils::createBoard(
        "       ♚"
        "        "
        "        "
        "        "
        "        "
        "        "
        "        "
        "    ♔   ");
        auto kingMoves = map({"Ke1d1", "Ke1d2", "Ke1e2", "Ke1f1", "Ke1f2"}, WHITE, board);
        auto allNotBeating = kingMoves;
        auto allMoves = allNotBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
        Board board = utils::createBoard(
        "       ♔"
        "        "
        "        "
        "        "
        "        "
        "        "
        "        "
        "♚       ");
        auto kingMoves = map({"Kh8g8", "Kh8g7", "Kh8h7"}, WHITE, board);
        auto allNotBeating = kingMoves;
        auto allMoves = allNotBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
        Board board = utils::createBoard(
        "       ♚"
        "        "
        "        "
        "        "
        "        "
        "        "
        "        "
        "♔       ");
        auto kingMoves = map({"Ka1a2", "Ka1b1", "Ka1b2"}, WHITE, board);
        auto allNotBeating = kingMoves;
        auto allMoves = allNotBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
        Board board = utils::createBoard(
        "       ♚"
        "        "
        "        "
        "        "
        "♔       "
        "        "
        "        "
        "        ");
        auto kingMoves = map({"Ka4a3", "Ka4b3", "Ka4b4", "Ka4b5", "Ka4a5"}, WHITE, board);
        auto allNotBeating = kingMoves;
        auto allMoves = allNotBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
        Board board = utils::createBoard(
        "       ♚"
        "        "
        "        "
        "        "
        "     ♔  "
        "        "
        "        "
        "        ");
        auto kingMoves = map({"Kf4e4", "Kf4e5", "Kf4f5", "Kf4g5", "Kf4g4", "Kf4g3", "Kf4f3", "Kf4e3"}, WHITE, board);
        auto allNotBeating = kingMoves;
        auto allMoves = allNotBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
}

TEST(MoveGeneratorTestsV2, shouldCorrectlyHandleNotPinnedKnightNoCheck)
{
    {
        Board board = utils::createBoard(
        "       ♚"
        "        "
        "    ♘   "
        "        "
        "        "
        "        "
        "        "
        "    ♔   ");
        auto kingMoves = map({"Ke1d1", "Ke1d2", "Ke1e2", "Ke1f1", "Ke1f2"}, WHITE, board);
        auto knightMoves = map({"Ne6f8", "Ne6g7", "Ne6g5", "Ne6f4", "Ne6d4", "Ne6c5", "Ne6c7", "Ne6d8"}, WHITE, board);
        auto allNotBeating = kingMoves + knightMoves;
        auto allMoves = allNotBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
        Board board = utils::createBoard(
        "      ♘♔"
        "        "
        "        "
        "        "
        "        "
        "        "
        "        "
        "♚       ");
        auto kingMoves = map({"Kh8h7", "Kh8g7"}, WHITE, board);
        auto knightMoves = map({"Ng8e7", "Ng8f6", "Ng8h6"}, WHITE, board);
        auto allNotBeating = kingMoves + knightMoves;
        auto allMoves = allNotBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
        Board board = utils::createBoard(
        "       ♚"
        "        "
        "        "
        "        "
        "        "
        "        "
        "  ♘     "
        "♔       ");
        auto kingMoves = map({"Ka1a2", "Ka1b1", "Ka1b2"}, WHITE, board);
        auto knightMoves = map({"Nc2a3", "Nc2b4", "Nc2d4", "Nc2e3", "Nc2e1"}, WHITE, board);
        auto allNotBeating = kingMoves + knightMoves;
        auto allMoves = allNotBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
}

TEST(MoveGeneratorTestsV2, shouldNotMovePinnedKnight)
{
    {
        Board board = utils::createBoard(
        "    ♜  ♚"
        "        "
        "    ♘   "
        "        "
        "        "
        "        "
        "        "
        "    ♔   ");
        auto kingMoves = map({"Ke1d1", "Ke1d2", "Ke1e2", "Ke1f2", "Ke1f1"}, WHITE, board);
        auto allNotBeating = kingMoves;
        auto allMoves = allNotBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
        Board board = utils::createBoard(
        "♜     ♘♔"
        "        "
        "        "
        "        "
        "        "
        "        "
        "        "
        "♚       ");
        auto kingMoves = map({"Kh8h7", "Kh8g7"}, WHITE, board);
        auto allNotBeating = kingMoves;
        auto allMoves = allNotBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
        Board board = utils::createBoard(
        "♚       "
        "        "
        "        "
        "        "
        "        "
        "  ♝     "
        " ♘      "
        "♔       ");
        auto kingMoves = map({"Ka1a2", "Ka1b1"}, WHITE, board);
        auto allNotBeating = kingMoves;
        auto allMoves = allNotBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
}

TEST(MoveGeneratorTestsV2, shouldBeAbleToPreventCheckByKnight)
{
    {
        Board board = utils::createBoard(
        "♚   ♜♘  "
        "        "
        "        "
        "        "
        "        "
        "        "
        "        "
        "    ♔   ");
        auto kingMoves = map({"Ke1d1", "Ke1d2", "Ke1f1", "Ke1f2"}, WHITE, board);
        auto knightMoves = map({"Nf8e6"}, WHITE, board);
        auto allNotBeating = kingMoves + knightMoves;
        auto allMoves = allNotBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
        Board board = utils::createBoard(
        "♜      ♔"
        "  ♘     "
        "        "
        "        "
        "        "
        "        "
        "        "
        "♚       ");
        auto kingMoves = map({"Kh8h7", "Kh8g7"}, WHITE, board);
        auto knightMoves = map({"Nc7e8"}, WHITE, board);
        auto knightBeatingMoves = map({"Nc7xa8"}, WHITE, board);
        
        auto allNotBeating = kingMoves + knightMoves;
        auto allBeating = knightBeatingMoves;
        auto allMoves = allNotBeating + allBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), allBeating);
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
        Board board = utils::createBoard(
        "♚       "
        "        "
        "        "
        "        "
        "♘       "
        "  ♝     "
        "        "
        "♔       ");
        auto kingMoves = map({"Ka1a2", "Ka1b1"}, WHITE, board);
        auto knightMoves = map({"Na4b2"}, WHITE, board);
        auto knightBeatingMoves = map({"Na4xc3"}, WHITE, board);

        auto allNotBeating = kingMoves + knightMoves;
        auto allBeating = knightBeatingMoves;
        auto allMoves = allNotBeating + allBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), allBeating);
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
}

TEST(MoveGeneratorTestsV2, canMoveOnlyKingWhenThereAre2Attackers)
{
    {
        Board board = utils::createBoard(
        "♚   ♜♘  "
        "        "
        "        "
        "        "
        "        "
        "  ♝     "
        "        "
        "    ♔   ");
        auto kingMoves = map({"Ke1d1", "Ke1f1", "Ke1f2"}, WHITE, board);
        auto allMoves = kingMoves;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allMoves));
    }
    {
        Board board = utils::createBoard(
        "♜      ♔"
        "  ♘   ♝ "
        "        "
        "        "
        "        "
        "        "
        "        "
        "♚       ");
        auto kingMoves = map({"Kh8h7"}, WHITE, board);
        auto kingBeatingMoves = map({"Kh8xg7"}, WHITE, board);

        auto allNotBeating = kingMoves;
        auto allBeating = kingBeatingMoves;
        auto allMoves = allNotBeating + allBeating;
        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), allBeating);
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
        Board board = utils::createBoard(
        "♜      ♔"
        "  ♘   ♝ "
        "     ♝  "
        "        "
        "        "
        "        "
        "        "
        "♚       ");
        auto kingMoves = map({"Kh8h7"}, WHITE, board);
        auto allNotBeating = kingMoves;
        auto allMoves = allNotBeating;
    
        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
        Board board = utils::createBoard(
        "♜      ♔"
        "  ♘     "
        "     ♝  "
        "        "
        "        "
        "        "
        "        "
        "♚       ");
        auto kingMoves = map({"Kh8h7"}, WHITE, board);
        auto allNotBeating = kingMoves;
        auto allMoves = allNotBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
        Board board = utils::createBoard(
        "       ♚"
        "        "
        "        "
        "        "
        "♘       "
        "  ♝     "
        "        "
        "♔      ♜");
        auto kingMoves = map({"Ka1a2"}, WHITE, board);
        auto allNotBeating = kingMoves;
        auto allMoves = allNotBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
}

TEST(MoveGeneratorTestsV2, shouldEvaluateSinglePawnMoves)
{
    {
        Board board = utils::createBoard(
        "        "
        "        "
        "        "
        "        "
        "   ♙    "
        "        "
        "        "
        "♚   ♔   ");
        auto kingMoves = map({"Ke1d1", "Ke1d2", "Ke1e2", "Ke1f2", "Ke1f1"}, WHITE, board);
        auto pawnMoves = map({"d4d5"}, WHITE, board);
        auto allNotBeating = kingMoves + pawnMoves;
        auto allMoves = allNotBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
    Board board = utils::createBoard(
        "        "
        "        "
        "        "
        "        "
        "   ♙    "
        "♙    ♙ ♙"
        "♙    ♙  "
        "    ♔   ");
        auto kingMoves = map({"Ke1d1", "Ke1d2", "Ke1e2", "Ke1f1"}, WHITE, board);
        auto pawnMoves = map({"a3a4", "d4d5", "f3f4", "h3h4"}, WHITE, board);
        auto allNotBeating = kingMoves + pawnMoves;
        auto allMoves = allNotBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
    Board board = utils::createBoard(
        "   ♚    "
        "        "
        "        "
        "        "
        "        "
        "        "
        "   ♟    "
        "        ", BLACK);
        auto kingMoves = map({"Kd8c8", "Kd8c7", "Kd8d7", "Kd8e8", "Kd8e7"}, BLACK, board);
        auto pawnsWeakPromotingMovesMoves = map({"d2d1=R", "d2d1=B", "d2d1=N"}, BLACK, board);
        auto allNotBeating = kingMoves + pawnsWeakPromotingMovesMoves;

        auto pawnsPromotingMoves = map({"d2d1=Q"}, BLACK, board);
        auto allBeating = pawnsPromotingMoves;
        auto allMoves = allNotBeating + allBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, BLACK);
        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size() - pawnsWeakPromotingMovesMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), allBeating);
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
    Board board = utils::createBoard(
        "    ♚   "
        "♟    ♟  "
        "♟    ♟ ♟"
        "   ♟    "
        "   ♙    "
        "♙    ♙ ♙"
        "♙    ♙  "
        "    ♔   ", BLACK);
        auto kingMoves = map({"Ke8d8", "Ke8d7", "Ke8e7", "Ke8f8"}, BLACK, board);
        auto pawnMoves = map({"a6a5", "f6f5", "h6h5"}, BLACK, board);
        auto allNotBeating = kingMoves + pawnMoves;
        auto allMoves = allNotBeating ;

        MoveGenerator::MoveGeneratorV2 sut(board, BLACK);
        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));

    }
}

TEST(MoveGeneratorTestsV2, shouldEvaluateDoublePawnMoves)
{
    {
        Board board = utils::createBoard(
            "        "
            "        "
            "        "
            "        "
            "   ♙    "
            "♙      ♙"
            "♙  ♙ ♙  "
            "    ♔   ");
        auto kingMoves = map({"Ke1d1", "Ke1e2", "Ke1f1"}, WHITE, board);
        auto pawnMoves = map({"a3a4", "d2d3", "d4d5", "f2f3", "f2f4", "h3h4"}, WHITE, board);
        auto allNotBeating = kingMoves + pawnMoves;
        auto allMoves = allNotBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
        Board board = utils::createBoard(
            "    ♚   "
            "♟  ♟ ♟  "
            "♟      ♟"
            "   ♟    "
            "        "
            "        "
            "        "
            "        ", BLACK);
        auto kingMoves = map({"Ke8d8", "Ke8e7", "Ke8f8"}, BLACK, board);
        auto pawnMoves = map({"a6a5", "d7d6", "d5d4", "f7f6", "f7f5", "h6h5"}, BLACK, board);
        auto allNotBeating = kingMoves + pawnMoves;
        auto allMoves = allNotBeating ;

        MoveGenerator::MoveGeneratorV2 sut(board, BLACK);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
}

TEST(MoveGeneratorTestsV2, shouldNotDoNotLegalMovesForPinnedPawns)
{
    {
        Board board = utils::createBoard(
            "        "
            "        "
            "        "
            "       ♝"
            "        "
            "     ♙  "
            "♜  ♙♔   "
            "        ");
        auto kingMoves = map({"Ke2d1", "Ke2e1", "Ke2f1", "Ke2f2", "Ke2d3", "Ke2e3"}, WHITE, board);
        auto allNotBeating = kingMoves;
        auto allMoves = allNotBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
        Board board = utils::createBoard(
            "    ♜   "
            "        "
            "        "
            "        "
            "       ♝"
            "        "
            "    ♙♙  "
            "    ♔   ");
        auto kingMoves = map({"Ke1d1", "Ke1d2", "Ke1f1"}, WHITE, board);
        auto pawnMoves = map({"e2e3", "e2e4"}, WHITE, board);
        auto allNotBeating = kingMoves + pawnMoves;
        auto allMoves = allNotBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
        Board board = utils::createBoard(
            "        "
            "♖  ♟♚♟♖ "
            "     ♟  "
            "        "
            "       ♗"
            "        "
            "        "
            "        ", BLACK);
        auto kingMoves = map({"Ke7d8", "Ke7e8", "Ke7f8", "Ke7d6", "Ke7e6"}, BLACK, board);
        auto allNotBeating = kingMoves;
        auto allMoves = allNotBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, BLACK);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
        Board board = utils::createBoard(
            "    ♚   "
            "    ♟   "
            "        "
            "        "
            "        "
            "    ♖   "
            "        "
            "        ", BLACK);
        auto kingMoves = map({"Ke8d8", "Ke8d7", "Ke8f7", "Ke8f8"}, BLACK, board);
        auto pawnMoves = map({"e7e6", "e7e5"}, BLACK, board);
        auto allNotBeating = kingMoves + pawnMoves;
        auto allMoves = allNotBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, BLACK);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
}

TEST(MoveGeneratorTestsV2, shouldPerformStandardBeatings)
{
    {
        Board board = utils::createBoard(
            "        "
            "        "
            "        "
            "        "
            "      ♜ "
            "  ♜  ♙  "
            "   ♙♔   "
            "        ");
        auto pawnMoves = map({"d2d3", "d2d4", "f3f4"}, WHITE, board);
        auto kingMoves = map({"Ke2d1", "Ke2e1", "Ke2f1","Ke2f2"}, WHITE, board);
        auto allNotBeating = kingMoves + pawnMoves;

        auto pawnsBeatingMoves = map({"d2xc3", "f3xg4"}, WHITE, board);
        auto allBeating = pawnsBeatingMoves;

        auto allMoves = allNotBeating + allBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), ::testing::UnorderedElementsAreArray(allBeating));
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
        Board board = utils::createBoard(
            "        "
            "       ♜"
            "♙       "
            "        "
            "♜       "
            "       ♙"
            "    ♔   "
            "        ");
        auto pawnMoves = map({"a6a7", "h3h4"}, WHITE, board);
        auto kingMoves = map({"Ke2d1", "Ke2e1", "Ke2f1", "Ke2f2", "Ke2d2", "Ke2d3", "Ke2e3", "Ke2f3"}, WHITE, board);
        auto allNotBeating = kingMoves + pawnMoves;
        auto allMoves = allNotBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
        Board board = utils::createBoard(
            "♚       "
            "    ♟♟  "
            "   ♗  ♗ "
            "        "
            "        "
            "        "
            "        "
            "        ", BLACK);
        auto kingMoves = map({"Ka8a7", "Ka8b7"}, BLACK, board);
        auto pawnMoves = map({"e7e6", "e7e5", "f7f6", "f7f5"}, BLACK, board);
        auto allNotBeating = kingMoves + pawnMoves;

        auto pawnsBeatingMoves = map({"e7xd6", "f7xg6"}, BLACK, board);
        auto allBeating = pawnsBeatingMoves;

        auto allMoves = allNotBeating + allBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, BLACK);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), ::testing::UnorderedElementsAreArray(allBeating));
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
        Board board = utils::createBoard(
            "♚       "
            "        "
            "        "
            "        "
            "        "
            "♟      ♟"
            "♖      ♖"
            "        ", BLACK);
        auto kingMoves = map({"Ka8a7", "Ka8b7", "Ka8b8"}, BLACK, board);
        auto allNotBeating = kingMoves;
        auto allMoves = allNotBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, BLACK);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
}

TEST(MoveGeneratorTestsV2, shouldPerformBeatingUntilNotExposeAKing)
{
    {
        Board board = utils::createBoard(
            "        "
            "        "
            "        "
            "        "
            "  ♝ ♝ ♝ "
            "   ♙ ♙  "
            "    ♔   "
            "        ");
        auto kingMoves = map({"Ke2d1", "Ke2e1", "Ke2f1", "Ke2d2", "Ke2f2", "Ke2e3"}, WHITE, board);
        auto allNotBeating = kingMoves;

        auto pawnsBeatingMoves = map({"d3xc4", "f3xg4"}, WHITE, board);
        auto allBeating = pawnsBeatingMoves;
        auto allMoves = allNotBeating + allBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), ::testing::UnorderedElementsAreArray(allBeating));
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
        Board board = utils::createBoard(
            "    ♚   "
            "   ♟ ♟  "
            "  ♗ ♗ ♗ "
            "        "
            "        "
            "        "
            "        "
            "        ", BLACK);
        auto kingMoves = map({"Ke8d8", "Ke8f8", "Ke8e7"}, BLACK, board);
        auto allNotBeating = kingMoves;

        auto pawnsBeatingMoves = map({"d7xc6", "f7xg6"}, BLACK, board);
        auto allBeating = pawnsBeatingMoves;

        auto allMoves = allNotBeating + allBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, BLACK);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), ::testing::UnorderedElementsAreArray(allBeating));
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
}

TEST(MoveGeneratorTestsV2, pawnsShouldBeAbleToBlockCheck)
{
    {
        Board board = utils::createBoard(
            "        "
            "        "
            "        "
            "        "
            "      ♝ "
            "        "
            "    ♔♙  "
            "        ");
        auto kingMoves = map({"Ke2e3", "Ke2d3", "Ke2d2", "Ke2e1", "Ke2f1"}, WHITE, board);
        auto pawnMoves = map({"f2f3"}, WHITE, board);
        auto allNotBeating = kingMoves + pawnMoves;

        auto allMoves = allNotBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
        Board board = utils::createBoard(
            "        "
            "        "
            "        "
            "        "
            "    ♔ ♜ "
            "        "
            "     ♙  "
            "        ");
        auto kingMoves = map({"Ke4d3", "Ke4e3", "Ke4f3", "Ke4d5", "Ke4e5", "Ke4f5"}, WHITE, board);
        auto pawnMoves = map({"f2f4"}, WHITE, board);
        auto allNotBeating = kingMoves + pawnMoves;

        auto allMoves = allNotBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
}

TEST(MoveGeneratorTestsV2, pawnsShouldBeAbleToBeatCheckFigure)
{
    {
        Board board = utils::createBoard(
            "        "
            "        "
            "        "
            "        "
            "      ♝ "
            "       ♙"
            "    ♔   "
            "        ");
        auto kingMoves = map({"Ke2e1", "Ke2f1", "Ke2f2", "Ke2e3", "Ke2d3", "Ke2d2"}, WHITE, board);
        auto allNotBeating = kingMoves;

        auto pawnsBeatingMoves = map({"h3xg4"}, WHITE, board);
        auto allBeating = pawnsBeatingMoves;

        auto allMoves = allNotBeating + allBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), ::testing::UnorderedElementsAreArray(allBeating));
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
        Board board = utils::createBoard(
            "        "
            "        "
            "        "
            "        "
            "  ♔  ♜  "
            "      ♙ "
            "        "
            "        ");
        auto kingMoves = map({"Kc4b5", "Kc4c5", "Kc4d5", "Kc4b3", "Kc4c3", "Kc4d3"}, WHITE, board);
        auto allNotBeating = kingMoves;

        auto pawnsBeatingMoves = map({"g3xf4"}, WHITE, board);
        auto allBeating = pawnsBeatingMoves;

        auto allMoves = allNotBeating + allBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), ::testing::UnorderedElementsAreArray(allBeating));
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
        Board board = utils::createBoard(
            "        "
            "        "
            "        "
            "        "
            "    ♔ ♜ "
            "     ♙  "
            "        "
            "        ");
        auto kingMoves = map({"Ke4d3", "Ke4e3", "Ke4d5", "Ke4e5", "Ke4f5"}, WHITE, board);
        auto pawnMoves = map({"f3f4"}, WHITE, board);
        auto allNotBeating = kingMoves + pawnMoves;

        auto pawnsBeatingMoves = map({"f3xg4"}, WHITE, board);
        auto allBeating = pawnsBeatingMoves;

        auto allMoves = allNotBeating + allBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), ::testing::UnorderedElementsAreArray(allBeating));
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
}

TEST(MoveGeneratorTestsV2, pawnsShouldCorrectlyHandleEnPassant)
{
    {
        Board board = utils::createBoard(
            "        "
            "   ♟ ♟  "
            "        "
            "    ♙   "
            "        "
            "        "
            "        "
            "♔       ", BLACK);
        MoveApplier::applyMove(board, notations::coordinates::createExtendedMove("f7-f5", BLACK, board));  // en passant on f6

        auto kingMoves = map({"Ka1a2", "Ka1b1", "Ka1b2"}, WHITE, board);
        auto pawnMoves = map({"e5e6"}, WHITE, board);
        auto allNotBeating = kingMoves + pawnMoves;

        auto pawnsBeatingMoves = map({"e5xf6"}, WHITE, board);
        auto allBeating = pawnsBeatingMoves;

        auto allMoves = allNotBeating + allBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), ::testing::UnorderedElementsAreArray(allBeating));
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
        Board board = utils::createBoard(
            "        "
            "   ♟ ♟  "
            "        "
            "    ♙   "
            "        "
            "        "
            "        "
            "♔       ", BLACK);
        MoveApplier::applyMove(board, notations::coordinates::createExtendedMove("d7-d5", BLACK, board));  // en passant on d6

        auto kingMoves = map({"Ka1a2", "Ka1b1", "Ka1b2"}, WHITE, board);
        auto pawnMoves = map({"e5e6"}, WHITE, board);
        auto allNotBeating = kingMoves + pawnMoves;

        auto pawnsBeatingMoves = map({"e5xd6"}, WHITE, board);
        auto allBeating = pawnsBeatingMoves;

        auto allMoves = allNotBeating + allBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), ::testing::UnorderedElementsAreArray(allBeating));
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
        Board board = utils::createBoard(
            "        "
            "   ♟ ♟  "
            "        "
            "    ♙   "
            "    ♔   "
            "        "
            "        "
            "        ", BLACK);
        MoveApplier::applyMove(board, notations::coordinates::createExtendedMove("f7-f5", BLACK, board));  // en passant on f6

        auto kingMoves = map({"Ke4f4", "Ke4f3", "Ke4e3", "Ke4d3", "Ke4d4", "Ke4d5"}, WHITE, board);
        auto allNotBeating = kingMoves;

        auto pawnsBeatingMoves = map({"e5xf6"}, WHITE, board);
        auto kingBeatingMoves = map({"Ke4xf5"}, WHITE, board);
        auto allBeating = pawnsBeatingMoves + kingBeatingMoves;

        auto allMoves = allNotBeating + allBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), ::testing::UnorderedElementsAreArray(allBeating));
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
        Board board = utils::createBoard(
            "        "
            "   ♟ ♟  "
            "        "
            "    ♙   "
            "    ♔   "
            "        "
            "        "
            "        ", BLACK);
        MoveApplier::applyMove(board, notations::coordinates::createExtendedMove("d7-d5", BLACK, board));  // en passant on d6

        auto kingMoves = map({"Ke4f4", "Ke4f3", "Ke4e3", "Ke4d3", "Ke4d4", "Ke4f5"}, WHITE, board);
        auto allNotBeating = kingMoves;

        auto pawnsBeatingMoves = map({"e5xd6"}, WHITE, board);
        auto kingBeatingMoves = map({"Ke4xd5"}, WHITE, board);
        auto allBeating = pawnsBeatingMoves + kingBeatingMoves;

        auto allMoves = allNotBeating + allBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), ::testing::UnorderedElementsAreArray(allBeating));
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
        Board board = utils::createBoard(
            "♚       "
            "        "
            "        "
            "        "
            "    ♟   "
            "        "
            "   ♙ ♙  "
            "        ", WHITE);
        MoveApplier::applyMove(board, notations::coordinates::createExtendedMove("d2-d4", WHITE, board));  // en passant on f6

        auto kingMoves = map({"Ka8a7", "Ka8b7", "Ka8b8"}, BLACK, board);
        auto pawnMoves = map({"e4e3"}, BLACK, board);
        auto allNotBeating = kingMoves + pawnMoves;

        auto pawnsBeatingMoves = map({"e4xd3"}, BLACK, board);
        auto allBeating = pawnsBeatingMoves;
        auto allMoves = allNotBeating + allBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, BLACK);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), ::testing::UnorderedElementsAreArray(allBeating));
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
        Board board = utils::createBoard(
            "♚       "
            "        "
            "        "
            "        "
            "    ♟   "
            "        "
            "   ♙ ♙  "
            "        ", WHITE);
        MoveApplier::applyMove(board, notations::coordinates::createExtendedMove("f2-f4", WHITE, board));  // en passant on d6

        auto kingMoves = map({"Ka8a7", "Ka8b7", "Ka8b8"}, BLACK, board);
        auto pawnMoves = map({"e4e3"}, BLACK, board);
        auto allNotBeating = kingMoves + pawnMoves;

        auto pawnsBeatingMoves = map({"e4xf3"}, BLACK, board);
        auto allBeating = pawnsBeatingMoves;

        auto allMoves = allNotBeating + allBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, BLACK);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), ::testing::UnorderedElementsAreArray(allBeating));
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
        Board board = utils::createBoard(
            "        "
            "        "
            "        "
            "    ♚   "
            "    ♟   "
            "        "
            "   ♙ ♙  "
            "        ", WHITE);
        MoveApplier::applyMove(board, notations::coordinates::createExtendedMove("d2-d4", WHITE, board));  // en passant on f6

        auto kingMoves = map({"Ke5d5", "Ke5f5", "Ke5d6", "Ke5e6", "Ke5f6", "Ke5f4"}, BLACK, board);
        auto allNotBeating = kingMoves;

        auto pawnsBeatingMoves = map({"e4xd3"}, BLACK, board);
        auto kingBeatingMoves = map({"Ke5xd4"}, BLACK, board);
        auto allBeatings = pawnsBeatingMoves + kingBeatingMoves;

        auto allMoves = allNotBeating + allBeatings;

        MoveGenerator::MoveGeneratorV2 sut(board, BLACK);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), ::testing::UnorderedElementsAreArray(allBeatings));
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
        Board board = utils::createBoard(
            "        "
            "        "
            "        "
            "    ♚   "
            "    ♟   "
            "        "
            "   ♙ ♙  "
            "        ", WHITE);
        MoveApplier::applyMove(board, notations::coordinates::createExtendedMove("f2-f4", WHITE, board));  // en passant on d6

        auto kingMoves = map({"Ke5d5", "Ke5f5", "Ke5d6", "Ke5e6", "Ke5f6", "Ke5d4"}, BLACK, board);
        auto allNotBeating = kingMoves;

        auto pawnsBeatingMoves = map({"e4xf3"}, BLACK, board);
        auto kingBeatingMoves = map({"Ke5xf4"}, BLACK, board);
        auto allBeatings = pawnsBeatingMoves + kingBeatingMoves;

        auto allMoves = allNotBeating + allBeatings;

        MoveGenerator::MoveGeneratorV2 sut(board, BLACK);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), ::testing::UnorderedElementsAreArray(allBeatings));
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
}

TEST(MoveGeneratorTestsV2, shouldFindRockMoves)
{
    {
        Board board = utils::createBoard(
            "       ♖"
            "        "
            "♖       "
            "        "
            "        "
            "        "
            "        "
            "♔       ", WHITE);
        auto kingMoves = map({"Ka1a2", "Ka1b1", "Ka1b2"}, WHITE, board);
        auto rockMoves = map({"Ra6a2", "Ra6a3", "Ra6a4", "Ra6a5", "Ra6a7", "Ra6a8",
                                "Ra6b6", "Ra6c6", "Ra6d6", "Ra6e6", "Ra6f6", "Ra6g6", "Ra6h6",
                                "Rh8a8", "Rh8b8", "Rh8c8", "Rh8d8", "Rh8e8", "Rh8f8", "Rh8g8",
                                "Rh8h7", "Rh8h6", "Rh8h5", "Rh8h4", "Rh8h3", "Rh8h2", "Rh8h1"}, WHITE, board);
        auto allNotBeating = kingMoves + rockMoves;
        auto allMoves = allNotBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
        Board board = utils::createBoard(
            "   ♜   ♖"
            "        "
            "♖       "
            "        "
            "        "
            "        "
            "       ♜"
            "♔       ", WHITE);
        auto kingMoves = map({"Ka1b1"}, WHITE, board);
        auto rockMoves = map({"Ra6a2", "Ra6a3", "Ra6a4", "Ra6a5", "Ra6a7", "Ra6a8",
                                "Ra6b6", "Ra6c6", "Ra6d6", "Ra6e6", "Ra6f6", "Ra6g6", "Ra6h6",
                                "Rh8e8", "Rh8f8", "Rh8g8",
                                "Rh8h7", "Rh8h6", "Rh8h5", "Rh8h4", "Rh8h3"}, WHITE, board);
        auto allNotBeating = kingMoves + rockMoves;

        auto rockBeatingMoves = map({"Rh8xd8", "Rh8xh2"}, WHITE, board);
        auto allBeatings = rockBeatingMoves;

        auto allMoves = allNotBeating + allBeatings;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), ::testing::UnorderedElementsAreArray(allBeatings));
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
}

TEST(MoveGeneratorTestsV2, shouldCorrectlyHandlePinnedRocks)
{
    {
        Board board = utils::createBoard(
            "        "
            "♜       "
            "♖       "
            "        "
            "        "
            "        "
            "        "
            "♔       ", WHITE);
        auto kingMoves = map({"Ka1a2", "Ka1b1", "Ka1b2"}, WHITE, board);
        auto rockMoves = map({"Ra6a2", "Ra6a3", "Ra6a4", "Ra6a5"}, WHITE, board);
        auto allNotBeating = kingMoves + rockMoves;

        auto rockBeatingMoves = map({"Ra6xa7"}, WHITE, board);
        auto allBeatings = rockBeatingMoves;

        auto allMoves = allNotBeating + allBeatings;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), ::testing::UnorderedElementsAreArray(allBeatings));
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
        Board board = utils::createBoard(
            "        "
            "        "
            "        "
            "        "
            "        "
            "  ♝     "
            " ♖      "
            "♔       ", WHITE);
        auto kingMoves = map({"Ka1a2", "Ka1b1"}, WHITE, board);
        auto allNotBeating = kingMoves;

        auto allMoves = allNotBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
}

TEST(MoveGeneratorTestsV2, rocksShouldBeAbleTOPreventCheck)
{
    {
        Board board = utils::createBoard(
            "        "
            "        "
            "♖       "
            "        "
            "        "
            "♜       "
            "        "
            "♔       ", WHITE);
        auto kingMoves = map({"Ka1b1", "Ka1b2"}, WHITE, board);
        auto allNotBeating = kingMoves;

        auto rockBeatingMoves = map({"Ra6xa3"}, WHITE, board);
        auto allBeatings = rockBeatingMoves;

        auto allMoves = allNotBeating + allBeatings;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), ::testing::UnorderedElementsAreArray(allBeatings));
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
    {
        Board board = utils::createBoard(
            "        "
            "        "
            "        "
            "        "
            "        "
            "♜       "
            "       ♖"
            "♔       ", WHITE);
        auto kingMoves = map({"Ka1b1", "Ka1b2"}, WHITE, board);
        auto rockMoves = map({"Rh2a2"}, WHITE, board);

        auto allNotBeating = kingMoves + rockMoves;

        auto allMoves = allNotBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }
}

TEST(MoveGeneratorTestsV2, shouldFindBishopoves)
{
    {
        Board board = utils::createBoard(
            "        "
            "        "
            "        "
            "        "
            "        "
            "        "
            " ♗      "
            "♔      ♗", WHITE);
        auto kingMoves = map({"Ka1b1", "Ka1a2"}, WHITE, board);
        auto rockMoves = map({"Bb2a3", "Bb2c1",
                                "Bb2c3", "Bb2d4", "Bb2e5", "Bb2f6", "Bb2g7", "Bb2h8",
                                "Bh1g2", "Bh1f3", "Bh1e4", "Bh1d5", "Bh1c6", "Bh1b7", "Bh1a8"}, WHITE, board);

        auto allNotBeating = kingMoves + rockMoves;

        auto allMoves = allNotBeating;

        MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

        ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
        ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
        ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
    }

}

////////////////////////////////////////////////////////////

TEST(MoveGeneratorTestsV2, shouldReturnInitialMoves)
{
    Board board = utils::createBoard(utils::InitialBoardString);
    auto pawnInitialMoves = map(
            {"a2a3", "a2a4", "b2b3", "b2b4", "c2c3", "c2c4", "d2d3",
            "d2d4", "e2e3", "e2e4", "f2f3", "f2f4", "g2g3", "g2g4",
            "h2h3", "h2h4"}, WHITE, board);
    auto knightInitialMoves = map(
            {"Nb1a3", "Nb1c3", "Ng1f3", "Ng1h3"}, WHITE, board);
    auto allNotBeating = pawnInitialMoves + knightInitialMoves;

    auto allMoves = allNotBeating;

    MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

    ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
    ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
    ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
}


TEST(MoveGeneratorTestsV2, shouldCorectlyAnalyzePos_2)
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
            {"b2b3", "b2b4", "c2c3", "c2c4",
              "g2g3", "g2g4",
             "h2h3", "h2h4"}, WHITE, board);
    auto knightMoves = map(
            {"Na3b5", "Na3c4", "Na3b1",
             "Nf3d2",  "Nf3g5", "Nf3h4", "Nf3g1"}, WHITE, board);

    auto rockMoves = map(
            {"Ra1b1", "Rh1g1"}, WHITE, board);

    auto bishopMoves = map(
            {"Bc1d2", "Bc1e3", "Bc1f4", "Bc1g5", "Bc1h6",
             "Bf1e2", "Bf1d3", "Bf1c4", "Bf1b5", "Bf1a6"}, WHITE, board);

    auto queenMoves = map(
            {"Qd1d2", "Qd1d3", "Qd1e2"}, WHITE, board);

    auto kingMoves = map(
            {"Ke1d2", "Ke1e2"}, WHITE, board);
    auto allNotBeating = pawnMoves+knightMoves+rockMoves+bishopMoves+queenMoves+kingMoves;

    auto pawnBeatingMoves = map({"d4xe5", "e4xd5", "e4xf5"}, WHITE, board);
    auto knightBeatingMoves = map({"Nf3xe5"}, WHITE, board);
    auto allBeatings = pawnBeatingMoves + knightBeatingMoves;

    auto allMoves = allNotBeating + allBeatings;

    MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

    ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
    ASSERT_THAT(sut.generateBeatingMoves(), ::testing::UnorderedElementsAreArray(allBeatings));
    ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
}

TEST(MoveGeneratorTestsV2, shouldCorectlyAnalyzePosWithMoves)
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
    MoveApplier::applyMove(board, notations::coordinates::createExtendedMove("e1-e2", WHITE, board));
    MoveApplier::applyMove(board, notations::coordinates::createExtendedMove("e8-e7", BLACK, board));
    MoveApplier::applyMove(board, notations::coordinates::createExtendedMove("e2-e1", WHITE, board));
    MoveApplier::applyMove(board, notations::coordinates::createExtendedMove("c7-c5", BLACK, board));

    auto pawnMoves = map({"b5b6", "e4e5", "h2h3", "h2h4"}, WHITE, board);
    auto rockMoves = map(
        {"Ra1a2", "Ra1a3", "Ra1a4", "Ra1a5", "Ra1a6", "Ra1a7", "Ra1a8",
        "Ra1b1", "Ra1c1", "Ra1d1",
        "Rh1g1", "Rh1f1"}, WHITE, board);
    auto kingMoves = map(
        {"Ke1d1", "Ke1d2", "Ke1e2", "Ke1f2", "Ke1f1"}, WHITE, board);
    auto allNotBeating = pawnMoves+rockMoves+kingMoves;

    const char* enPassant = "b5xc6";
    auto pawnsBeatingMoves = map({enPassant}, WHITE, board);
    auto allBeatings = pawnsBeatingMoves;

    auto allMoves = allNotBeating + allBeatings;

    MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

    ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
    ASSERT_THAT(sut.generateBeatingMoves(), ::testing::UnorderedElementsAreArray(allBeatings));
    ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
}

TEST(MoveGeneratorTestsV2, shouldAllowCastles)
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
    auto allNotBeating = pawnMoves+rockMoves+kingMoves;

    auto allMoves = allNotBeating;

    MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

    ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
    ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
    ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
}


TEST(MoveGeneratorTestsV2, shouldPreventIllegalCasles)
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
    auto allNotBeating = pawnMoves + rockMoves + kingMoves;

    auto allMoves = allNotBeating;

    MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

    ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
    ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
    ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
}

TEST(MoveGeneratorTestsV2, shouldPreventIllegalCasles_2)
{
    Board board = utils::createBoard(
        "        "
        "  ♟ ♚   "
        "        "
        " ♙      "
        "    ♙   "
        "        "
        "       ♙"
        "♖   ♔  ♖");

    MoveApplier::applyMove(board, notations::coordinates::createExtendedMove("a1-b1", WHITE, board));
    MoveApplier::applyMove(board, notations::coordinates::createExtendedMove("e7-e8", BLACK, board));
    MoveApplier::applyMove(board, notations::coordinates::createExtendedMove("b1-a1", WHITE, board));
    MoveApplier::applyMove(board, notations::coordinates::createExtendedMove("e8-e7", BLACK, board));

    auto pawnMoves = map(
        {"b5b6", "e4e5", "h2h3", "h2h4"}, WHITE, board);
    auto rockMoves = map(
        {"Ra1a2", "Ra1a3", "Ra1a4", "Ra1a5", "Ra1a6", "Ra1a7", "Ra1a8",
        "Ra1b1", "Ra1c1", "Ra1d1",
        "Rh1g1", "Rh1f1"}, WHITE, board);
    const char* shortCastle = "O-O";
    auto kingMoves = map(
        {shortCastle, "Ke1e2", "Ke1f2", "Ke1f1", "Ke1d2", "Ke1d1"}, WHITE, board);
    auto allNotBeating = kingMoves + rockMoves + pawnMoves;

    auto allMoves = allNotBeating;

    MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

    ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
    ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
    ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
}

TEST(MoveGeneratorTestsV2, shouldPreventIllegalCasles_3)
{
    Board board = utils::createBoard(
        "        "
        "  ♟ ♚   "
        "        "
        " ♙      "
        "    ♙   "
        "        "
        "       ♙"
        "♖   ♔  ♖");

    MoveApplier::applyMove(board, notations::coordinates::createExtendedMove("h1-g1", WHITE, board));
    MoveApplier::applyMove(board, notations::coordinates::createExtendedMove("e7-e8", BLACK, board));
    MoveApplier::applyMove(board, notations::coordinates::createExtendedMove("g1-h1", WHITE, board));
    MoveApplier::applyMove(board, notations::coordinates::createExtendedMove("e8-e7", BLACK, board));

    auto pawnMoves = map(
        {"b5b6", "e4e5", "h2h3", "h2h4"}, WHITE, board);
    auto rockMoves = map(
        {"Ra1a2", "Ra1a3", "Ra1a4", "Ra1a5", "Ra1a6", "Ra1a7", "Ra1a8",
        "Ra1b1", "Ra1c1", "Ra1d1",
        "Rh1g1", "Rh1f1"}, WHITE, board);
    const char* longCastle = "O-O-O";
    auto kingMoves = map(
        {longCastle, "Ke1e2", "Ke1f2", "Ke1f1", "Ke1d2", "Ke1d1"}, WHITE, board);
    auto allNotBeating = kingMoves + rockMoves + pawnMoves;

    auto allMoves = allNotBeating;

    MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

    ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
    ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
    ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
}

TEST(MoveGeneratorTestsV2, shouldFindPromotions)
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
    auto kingMoves = map(
        {"Kd1c1", "Kd1c2", "Kd1d2"}, WHITE, board);
    auto pawnsWeakPromotingMovesMoves = map(
        {"d7xc8=N", "d7xc8=B", "d7xc8=R",
        "d7d8=N", "d7d8=B", "d7d8=R",
        "d7xe8=N", "d7xe8=B", "d7xe8=R"}, WHITE, board);
    auto allNotBeating = kingMoves + pawnsWeakPromotingMovesMoves;

    auto pawnMoves = map(
        {"d7xc8=Q",
        "d7d8=Q",
        "d7xe8=Q",}, WHITE, board);
    auto allBeatings = pawnMoves;

    auto allMoves = allBeatings + allNotBeating;

    MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

    ASSERT_EQ(sut.getValidMoveCount(), allMoves.size() - pawnsWeakPromotingMovesMoves.size());
    ASSERT_THAT(sut.generateBeatingMoves(), ::testing::UnorderedElementsAreArray(allBeatings));
    ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
}

TEST(MoveGeneratorTestsV2, shouldCorectlyAnalyzePosWithCheck)
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

    auto pawnBeatingMoves = map(
        {"h5xg6"}, WHITE, board);

    auto knightMoves = map(
        {"Nf3g5"}, WHITE, board);

    auto rockMoves = map(
        {"Rh4g4"}, WHITE, board);

    auto bishopMoves = map(
        {"Bc1g5"}, WHITE, board);

    auto kingMoves = map(
        {"Kg3h3", "Kg3h2"}, WHITE, board);
    auto allMoves = pawnBeatingMoves + knightMoves + rockMoves + kingMoves + bishopMoves;

    MoveGenerator::MoveGeneratorV2 sut(board, WHITE);
    ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
    ASSERT_THAT(sut.generateBeatingMoves(), ::testing::UnorderedElementsAreArray(pawnBeatingMoves));
    // ASSERT_THAT(sut.generate(board, WHITE),
    //     ::testing::UnorderedElementsAreArray(pawnMoves+knightMoves+bishopMoves+rockMoves+kingMoves));
}
/*
TEST(MoveGeneratorTestsV2, shouldFindInitialMovesForBlack)
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


TEST(MoveGeneratorTestsV2, shouldCorectlyAnalyzePosForBlack)
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

    auto knightMoves = map(
        {"Na6b8", "Na6c5", "Na6b4",
        "Ng4xh2", "Ng4xf2", "Ng4e3", "Ng4e5", "Ng4f6", "Ng4h6"}, BLACK, board);

    auto rockMoves = map(
        {"Ra8a7", "Ra8b8", "Rh8g8"}, BLACK, board);


    auto bishopMoves = map(
        {"Bc8d7", "Bc8e6", "Bc8f5",
        "Bf8g7", "Bf8h6"}, BLACK, board);

    auto queenMoves = map(
        {"Qd8d7", "Qd8d6"}, BLACK, board);

    auto kingMoves = map(
        {"Ke8d7"}, BLACK,  board);

    ASSERT_THAT(sut.generate(board, BLACK),
        ::testing::UnorderedElementsAreArray(pawnMoves+knightMoves+rockMoves+bishopMoves+queenMoves+kingMoves));
}

TEST(MoveGeneratorTestsV2,  shouldCorectlyAnalyzePosForBlack_2)
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

TEST(MoveGeneratorTestsV2, shouldFindBlackPromotions)
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

TEST(MoveGeneratorTestsV2, shallNotAllowToMovePinned)
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
             "Rd5c5", "Rd5b5", "Rd5xa5",
             "Rf5xg5"}, WHITE, board);

    auto kingMoves = map(
            {"Ke5f6", "Ke5d6", "Ke5d4", "Ke5f4"}, WHITE, board);

    ASSERT_THAT(sut.generate(board, WHITE),
                ::testing::UnorderedElementsAreArray(rockMoves + kingMoves));
}

TEST(MoveGeneratorTestsV2, shallNotAllowToMovePinnedBlack)
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

TEST(MoveGeneratorTestsV2, shallNotAllowToMoveDiagonal)
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

TEST(MoveGeneratorTestsV2, shallNotAllowToMoveDiagonalBlack)
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

TEST(MoveGeneratorTestsV2, shallNotAllowToMovePawnWhenPinned)
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
TEST(MoveGeneratorTestsV2, Custom_1)
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

TEST(MoveGeneratorTestsV2, Custom_2)
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

TEST(MoveGeneratorTestsV2, Custom_3)
{
    auto board = utils::createBoard("♜  ♘ ♜♚ "
                                    "♟ ♟♝ ♟♟ "
                                    "♕      ♟"
                                    "  ♟     "
                                    "   ♟    "
                                    " ♙  ♙   "
                                    "♙♗♙♙ ♙ ♙"
                                    "♖   ♔  ♛");

    auto kingMoves = map(
            {"Ke1e2"}, WHITE, board);
    auto queenMoves = map(
            {"Qa6f1"}, WHITE, board);

    ASSERT_THAT(sut.generate(board, WHITE),
                ::testing::UnorderedElementsAreArray(kingMoves + queenMoves));
}

TEST(MoveGeneratorTestsV2, Custom_4)
{

    auto board = utils::createBoard("♜   ♚  ♜"
                                    "♟ ♟♟ ♟♟♟"
                                    "♝♟♞ ♟ ♛ "
                                    "   ♙♙   "
                                    "  ♙     "
                                    "♗ ♙  ♘  "
                                    "♙    ♙♙♙"
                                    "♖  ♕ ♖♔ ", BLACK);
    auto rockMoves = map(
            {"Ra8b8", "Ra8c8", "Ra8d8",
             "Rh8g8", "Rh8f8"}, BLACK, board);

    auto kingMoves = map(
            {"O-O-O",
             "Ke8d8"}, BLACK, board);

    auto pawnMoves = map(
            {"b6b5", "d7d6", "e6xd5", "f7f6", "f7f5", "h7h6", "h7h5"}, BLACK, board);

    auto bishopMoves = map(
            {"Ba6b5", "Ba6xc4", "Ba6b7", "Ba6c8"}, BLACK, board);

    auto knightMoves = map(
            {"Nc6b8", "Nc6d8", "Nc6e7", "Nc6xe5", "Nc6d4", "Nc6b4", "Nc6a5" }, BLACK, board);

    auto queenMoves = map(
            {"Qg6f6", "Qg6h6",
             "Qg6g5", "Qg6g4", "Qg6g3", "Qg6xg2",
             "Qg6f5", "Qg6e4", "Qg6d3", "Qg6c2", "Qg6b1",
             "Qg6h5"}, BLACK, board);

    auto moves = sut.generate(board, BLACK);
    auto expectation = rockMoves + kingMoves + pawnMoves + bishopMoves + knightMoves + queenMoves;
    ASSERT_THAT(moves, ::testing::UnorderedElementsAreArray(expectation));
}

TEST(MoveGeneratorTestsV2, Custom_5)
{
    Board board = utils::createBoard(
                "♜♞..♚♝.."
                "♟♟...♟♟."
                "..♟..♞.♜"
                "...♟♟.♘♟"
                ".♗♗....."
                ".♙♘.♙..."
                "♙.♝♙.♙♙♙"
                "♖...♔..♖"
                , WHITE);

    auto moves = sut.generate(board, WHITE);
}

TEST(MoveGeneratorTestsV2, Custom_6)
{
    Board board = utils::createBoard(
                "♜♞..♚♝♞♜"
                "♟♟...♟♙."
                "..♟.♝..."
                ".......♟"
                "........"
                "..♘.♙♟.."
                "♙♙♙..♙♙♙"
                "♖.♗.♔♗.♖"
                , WHITE);

    auto moves = sut.generate(board, WHITE);

    auto rockMoves = map(
            {"Ra1b1", "Rh1g1"}, WHITE, board);

    auto kingMoves = map(
            {"Ke1d1", "Ke1d2"}, WHITE, board);

    auto pawnMoves = map(
            {"a2a3", "a2a4", "b2b3", "b2b4", "e3e4", "g2g3", "g2g4", "h2h3", "h2h4",
                "g2xf3", "g7xf8=Q", "g7xf8=N", "g7xf8=B", "g7xf8=R",
                "g7xh8=Q", "g7xh8=N", "g7xh8=B", "g7xh8=R"}, WHITE, board);

    auto bishopMoves = map(
            {"Bc1d2", "Bf1e2", "Bf1d3", "Bf1c4", "Bf1b5", "Bf1a6"}, WHITE, board);

    auto knightMoves = map(
            {"Nc3a4", "Nc3b5", "Nc3d5", "Nc3e4", "Nc3e2", "Nc3b1", "Nc3d1" }, WHITE, board);

    auto expectation = rockMoves + kingMoves + pawnMoves + bishopMoves + knightMoves;
    ASSERT_THAT(moves, ::testing::UnorderedElementsAreArray(expectation));
}

*/

TEST(MoveGeneratorTestsV2, Custom_7)
{
    Board board = utils::createBoard(
		"♜♞♝♛♚.♞♜" 
		"♟♟♟♟.♟♟♟" 
		"....♟..." 
		"........" 
		".♝.♙♙..." 
		"........" 
		"♙♙♙..♙♙♙" 
		"♖♘♗♕♔♗♘♖" , WHITE);
    auto kingMoves = map(
        {"Ke1e2"}, WHITE, board);
    auto pawnMoves = map(
        {"c2c3"}, WHITE, board);
    auto knightMoves = map(
        {"Nb1c3", "Nb1d2"}, WHITE, board);
    auto bishopMoves = map(
        {"Bc1d2"}, WHITE, board);
    auto queenMoves = map(
        {"Qd1d2"}, WHITE, board);
    auto allNotBeating = kingMoves + pawnMoves + knightMoves + bishopMoves + queenMoves;

    auto allMoves = allNotBeating;

    MoveGenerator::MoveGeneratorV2 sut(board, WHITE);
    
    ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
    ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>{});
    ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
}

TEST(MoveGeneratorTestsV2, Custom_8)
{
    Board board = utils::createBoard(
        "♜♞♝♛♚♝♞♜" 
        "♟♟♟.♟♟♟♟" 
        "........" 
        "........" 
        "....♙..." 
        "♙....♕.." 
        ".♙♙..♙♙♙" 
        "♖.♛♔.♗♘♖", WHITE);
    auto kingMoves = map(
        {"Kd1e2"}, WHITE, board);
    auto allNotBeating = kingMoves;

    auto kingBeatingMoves = map(
        {"Kd1xc1"}, WHITE, board);
    auto allBeating = kingBeatingMoves;

    auto allMoves = allNotBeating + allBeating;

    MoveGenerator::MoveGeneratorV2 sut(board, WHITE);

    ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
    ASSERT_THAT(sut.generateBeatingMoves(), ::testing::UnorderedElementsAreArray(allBeating));
    ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
}

TEST(MoveGeneratorTestsV2, shallNotExposeKingWhileEnPasantBeating)
{
    Board board = utils::createBoard(
        "        "
        "        "
        "        "
        "        "
        "♖♟♙    ♚"
        "        "
        "       ♔"
        "        ", BLACK);
    utils::setValidEnPassant(board, "c3");
    auto kingMoves = map(
        {"Kh4g4", "Kh4h5", "Kh4g5"}, BLACK, board);
    auto pawnMoves = map({"b4b3"}, BLACK, board);

    auto allNotBeating = kingMoves + pawnMoves;
    auto allMoves = allNotBeating;

    MoveGenerator::MoveGeneratorV2 sut(board, BLACK);
    ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
    ASSERT_THAT(sut.generateBeatingMoves(), std::vector<ExtendedMove>());
    ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
}

TEST(MoveGeneratorTestsV2, Custom_9)
{
    Board board = utils::createBoard(
            "        "
            "        "
            "        "
            "♚♟      "
            "♟ ♙     "
            " ♗    ♔ "
            "♙♙♙     "
            "        ", BLACK);


    auto pawnMoves = map({"a4a3", "b5b4"}, BLACK, board);
    auto kingMoves = map(
        {"Ka5a6", "Ka5b6", "Ka5b4"}, BLACK, board);
    auto allNotBeating = pawnMoves + kingMoves;

    auto pawnBeatingMoves = map(
        {"a4xb3", "b5xc4"}, BLACK, board);
    auto allBeating = pawnBeatingMoves;

    auto allMoves = allBeating + allNotBeating;

    MoveGenerator::MoveGeneratorV2 sut(board, BLACK);

    ASSERT_EQ(sut.getValidMoveCount(), allMoves.size());
    ASSERT_THAT(sut.generateBeatingMoves(), ::testing::UnorderedElementsAreArray(allBeating));
    ASSERT_THAT(sut.generateNonBeatingMoves(), ::testing::UnorderedElementsAreArray(allNotBeating));
}
