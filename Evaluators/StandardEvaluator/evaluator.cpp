#include <evaluatorIf.hpp>
#include <Common/MatherialEvaluator.hpp>
#include <Common/PawnStructureEvaluator.hpp>
#include <Common/SquareTablesEvaluator.hpp>

#include <detail/bitboardslookups.hpp>
#include <publicIf/NotationConversions.hpp>

namespace
{

matherial_evaluator::VALUES_TYPE piecesValues(
        TPawnValue(100),
        TKnightValue(300),
        TBishopValue(300),
        TRockValue(500),
        TQueenValue(900));

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
    return (playerOnMoveMovesCount - oponentMovesCount) * 10;
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

}

int evaluatePosition(BoardEngine& be, unsigned int validMovesCount)
{
    auto gameResult = be.getREsultWithoutRepeatitionCheck(validMovesCount);
    if ((gameResult == Result::whiteWon) | (gameResult == Result::blackWon))
    {
        return -10000000;
    }

    auto oponentValidMoves = be .generateValidMoveCount(be.board.playerOnMove + 1);
    return matherial_evaluator::evaluate(be.board, be.board.playerOnMove, piecesValues)
        + evaluateMoveCount(validMovesCount, oponentValidMoves)
        + pawn_structure_evaluator::evaluatePawnStructure(be.board, be.board.playerOnMove, pawnStructureCoeffincients)
        + evaluateDualBishop(be.board, be.board.playerOnMove)
        + square_tables_evaluator::evaluate(be.board, be.board.playerOnMove, squareTables);
}
