#include <evaluatorIf.hpp>
#include <Common/Positions.hpp>
#include <Common/MatherialEvaluator.hpp>
#include <Common/PawnStructureEvaluator.hpp>
#include <Common/SquareTablesEvaluator.hpp>
#include <MoveGeneratorV2/MoveGeneratorV2.hpp>

#include <detail/bitboardslookups.hpp>
#include <publicIf/NotationConversions.hpp>

#include <immintrin.h>

namespace
{

namespace dualbishop_evaluator
{

template<NOTATION::COLOR::color C>
void fill(short* out, const Board& board);

template<>
void fill<NOTATION::COLOR::color::white>(short* out, const Board& board)
{
    auto whiteDualBishipPremium =  50 * (2 == __builtin_popcountll(board.piecesBitSets[NOTATION::COLOR::WHITE].bishopsMask));
    auto blackDualBishipPremium =  50 * (2 == __builtin_popcountll(board.piecesBitSets[NOTATION::COLOR::BLACK].bishopsMask));

    out[DualBishopOffset] =  whiteDualBishipPremium - blackDualBishipPremium;
}

template<>
void fill<NOTATION::COLOR::color::black>(short* out, const Board& board)
{
    auto whiteDualBishipPremium =  50 * (2 == __builtin_popcountll(board.piecesBitSets[NOTATION::COLOR::WHITE].bishopsMask));
    auto blackDualBishipPremium =  50 * (2 == __builtin_popcountll(board.piecesBitSets[NOTATION::COLOR::BLACK].bishopsMask));

    out[DualBishopOffset] = blackDualBishipPremium - whiteDualBishipPremium;
}

}  // namespace dualbishop_evaluator

alignas(8) short coefficients[NAllCoefficients];
alignas(8) short features[NAllCoefficients];

matherial_evaluator::VALUES_TYPE piecesValues(
        TPawnValue(100),
        TKnightValue(300),
        TBishopValue(300),
        TRockValue(500),
        TQueenValue(900));

void initCoefficientsForMaterial()
{
    coefficients[NMaterialOffset] = 100;
    coefficients[NMaterialOffset + 1] = 300;
    coefficients[NMaterialOffset + 2] = 300;
    coefficients[NMaterialOffset + 3] = 500;
    coefficients[NMaterialOffset + 4] = 900;
}

void initCoefficientsForMoveCount()
{
    coefficients[MoveCountOffset] = 10;
}

void initCoefficientsForDualBishop()
{
    coefficients[DualBishopOffset] = 50;
}

PawnStructureCoefficients pawnStructureCoeffincients = {-50,-50};
SuareTableCoeffictients squareTables{
    {0, 0, 0, 0, 0, 0, 0, 0, 10, 10, 10, 0, 0, 10, 10, 10, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 20, 20, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 10, 10, 10, 10, 10, 10, 10, 10, 50, 50, 50, 50, 50, 50, 50, 50, 50, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 10, 10, 10, 0, 0, 10, 10, 10, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 20, 20, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 10, 10, 10, 10, 10, 10, 10, 10, 50, 50, 50, 50, 50, 50, 50, 50, 50, 0, 0, 0, 0, 0, 0, 0},
    {-50, -40, -30, -30, -30, -30, -40, -50, -40, -20, 0, 5, 5, 0, -20, -40, -30, 5, 10, 15, 15, 10, 5, -30, -30, 0, 15, 20, 20, 15, 0, -30, 30, 5, 15, 20, 20, 15, 5, -30, -30, 0, 10, 15, 15, 10, 0, -30, -40, -20, 0, 0, 0, 0, -20, -40, -50, -40, -30, -30, -30, -30, -40, -50},
    {-50, -40, -30, -30, -30, -30, -40, -50, -40, -20, 0, 5, 5, 0, -20, -40, -30, 5, 10, 15, 15, 10, 5, -30, -30, 0, 15, 20, 20, 15, 0, -30, 30, 5, 15, 20, 20, 15, 5, -30, -30, 0, 10, 15, 15, 10, 0, -30, -40, -20, 0, 0, 0, 0, -20, -40, -50, -40, -30, -30, -30, -30, -40, -50},
    {20, 30, 10,  0,  0, 10, 30, 20,  20, 20,  0,  0,  0,  0, 20, 20, -10,-20,-20,-20,-20,-20,-20,-10, -20,-30,-30,-40,-40,-30,-30,-20, -30,-40,-40,-50,-50,-40,-40,-30, -30,-40,-40,-50,-50,-40,-40,-30, -30,-40,-40,-50,-50,-40,-40,-30, -30,-40,-40,-50,-50,-40,-40,-30},
    {20, 30, 10,  0,  0, 10, 30, 20,  20, 20,  0,  0,  0,  0, 20, 20, -10,-20,-20,-20,-20,-20,-20,-10, -20,-30,-30,-40,-40,-30,-30,-20, -30,-40,-40,-50,-50,-40,-40,-30, -30,-40,-40,-50,-50,-40,-40,-30, -30,-40,-40,-50,-50,-40,-40,-30, -30,-40,-40,-50,-50,-40,-40,-30},
    {-20,-10,-10,-10,-10,-10,-10,-20, -10,  5,  0,  0,  0,  0,  5,-10, -10, 10, 10, 10, 10, 10, 10,-10, -10,  0, 10, 10, 10, 10,  0,-10, -10,  5,  5, 10, 10,  5,  5,-10,-10,  0,  5, 10, 10,  5,  0,-10,-10,  0,  0,  0,  0,  0,  0,-10,-20,-10,-10,-10,-10,-10,-10,-20},
    {-20,-10,-10,-10,-10,-10,-10,-20, -10,  5,  0,  0,  0,  0,  5,-10, -10, 10, 10, 10, 10, 10, 10,-10, -10,  0, 10, 10, 10, 10,  0,-10, -10,  5,  5, 10, 10,  5,  5,-10,-10,  0,  5, 10, 10,  5,  0,-10,-10,  0,  0,  0,  0,  0,  0,-10,-20,-10,-10,-10,-10,-10,-10,-20},
    {0,  0,  0,  5,  5,  0,  0,  0,  -5,  0,  0,  0,  0,  0,  0, -5,  -5,  0,  0,  0,  0,  0,  0, -5,  -5,  0,  0,  0,  0,  0,  0, -5,  -5,  0,  0,  0,  0,  0,  0, -5,  -5,  0,  0,  0,  0,  0,  0, -5,   5, 10, 10, 10, 10, 10, 10,  5,   0,  0,  0,  0,  0,  0,  0,  0},
    {0,  0,  0,  5,  5,  0,  0,  0,  -5,  0,  0,  0,  0,  0,  0, -5,  -5,  0,  0,  0,  0,  0,  0, -5,  -5,  0,  0,  0,  0,  0,  0, -5,  -5,  0,  0,  0,  0,  0,  0, -5,  -5,  0,  0,  0,  0,  0,  0, -5,   5, 10, 10, 10, 10, 10, 10,  5,   0,  0,  0,  0,  0,  0,  0,  0},
    {-20,-10,-10, -5, -5,-10,-10,-20, -10,  0,  5,  0,  0,  0,  0,-10, -10,  5,  5,  5,  5,  5,  0,-10,   0,  0,  5,  5,  5,  5,  0, -5,  -5,  0,  5,  5,  5,  5,  0, -5, -10,  0,  5,  5,  5,  5,  0,-10,-10,  0,  0,  0,  0,  0,  0,-10,-20,-10,-10, -5, -5,-10,-10,-20},
    {-20,-10,-10, -5, -5,-10,-10,-20, -10,  0,  5,  0,  0,  0,  0,-10, -10,  5,  5,  5,  5,  5,  0,-10,   0,  0,  5,  5,  5,  5,  0, -5,  -5,  0,  5,  5,  5,  5,  0, -5, -10,  0,  5,  5,  5,  5,  0,-10,-10,  0,  0,  0,  0,  0,  0,-10,-20,-10,-10, -5, -5,-10,-10,-20}
};

int evaluateMoveCount(int playerOnMoveMovesCount, int oponentMovesCount)
{
    return (playerOnMoveMovesCount - oponentMovesCount) * 2;
}

int evaluateDualBishop(const Board& board, NOTATION::COLOR::color playerOnMove)
{
    auto whiteDualBishipPremium =  50 * (2 == __builtin_popcountll(board.piecesBitSets[NOTATION::COLOR::WHITE].bishopsMask));
    auto blackDualBishipPremium =  50 * (2 == __builtin_popcountll(board.piecesBitSets[NOTATION::COLOR::BLACK].bishopsMask));
    if (playerOnMove == NOTATION::COLOR::color::black)
        return blackDualBishipPremium - whiteDualBishipPremium;
    return whiteDualBishipPremium - blackDualBishipPremium;
}

}  // namespace

void init(const char*)
{
    reverseTable(squareTables.white_pawn, squareTables.black_pawn);
    reverseTable(squareTables.white_knight, squareTables.black_knight);
    reverseTable(squareTables.white_king, squareTables.black_king);
    reverseTable(squareTables.white_bishop, squareTables.black_bishop);
    reverseTable(squareTables.white_rock, squareTables.black_rock);
    reverseTable(squareTables.white_queen, squareTables.black_queen);

    initCoefficientsForMaterial();
    initCoefficientsForMoveCount();
    initCoefficientsForDualBishop();
}

int evaluatePosition(BoardEngine& be, unsigned int validMovesCount)
{
    auto gameResult = be.getREsultWithoutRepeatitionCheck(validMovesCount);
    if ((gameResult == Result::whiteWon) | (gameResult == Result::blackWon))
    {
        return -10000000;
    }

    auto oponentValidMoves = be.getMoveGeneratorV2(be.board.playerOnMove + 1).getValidMoveCount();

    return matherial_evaluator::evaluate(be.board, be.board.playerOnMove, piecesValues)
        + evaluateMoveCount(validMovesCount, oponentValidMoves)
        + pawn_structure_evaluator::evaluatePawnStructure(be.board, be.board.playerOnMove, pawnStructureCoeffincients)
        + evaluateDualBishop(be.board, be.board.playerOnMove)
        + square_tables_evaluator::evaluate(be.board, be.board.playerOnMove, squareTables);
}

template<NOTATION::COLOR::color C>
void fill(const Board &board, unsigned int validMovesCount, unsigned int oponentValidMovesCount)
{
    matherial_evaluator::fill<C>(features,  board);
    features[MoveCountOffset] = validMovesCount - oponentValidMovesCount;
    dualbishop_evaluator::fill<C>(features, board);
}

/*
Something is not working well here.
*/
int evaluatePositionAvx(BoardEngine& be, unsigned int validMovesCount)
{
    auto gameResult = be.getREsultWithoutRepeatitionCheck(validMovesCount);
    if ((gameResult == Result::whiteWon) | (gameResult == Result::blackWon))
    {
        return -10000000;
    }

    auto oponentValidMoves = be.getMoveGeneratorV2(be.board.playerOnMove + 1).getValidMoveCount();

    if (be.board.playerOnMove == NOTATION::COLOR::color::white)
    {
        fill<NOTATION::COLOR::color::white>(be.board, validMovesCount, oponentValidMoves);
    }
    else
    {
        fill<NOTATION::COLOR::color::black>(be.board, validMovesCount, oponentValidMoves);
    }

    int sum = 0;
#ifdef USE_AVX
    __m128i coefficientsVec = _mm_load_si128((__m128i*)coefficients);
    __m128i featuresVec = _mm_load_si128((__m128i*)features);
    auto a = _mm_mullo_epi16(coefficientsVec, featuresVec);
    short out[8];
    _mm_store_si128((__m128i*)out, a);
    for (auto i = 0u; i < NAllCoefficients; ++i)
    {
        sum += out[i];
    }
#else
    for (auto i = 0u; i < NAllCoefficients; ++i)
    {
        sum += coefficients[i] * features[i];
    }
#endif
    return sum + square_tables_evaluator::evaluate(be.board, be.board.playerOnMove, squareTables)
        + pawn_structure_evaluator::evaluatePawnStructure(be.board, be.board.playerOnMove, pawnStructureCoeffincients);
}