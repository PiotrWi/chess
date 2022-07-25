#include <evaluatorIf.hpp>
#include <Common/MatherialEvaluator.hpp>

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

int evaluateMoveCount(int playerOnMoveMovesCount, int oponentMovesCount)
{
    return (playerOnMoveMovesCount - oponentMovesCount) * 10;
}

int evaluatePawnStructureForWhite(const Board & board)
{
    auto eval = 0;

    for (auto && rank : ranks)
    {
        auto pawnsInRank = __builtin_popcountll(
            rank & board.piecesBitSets[NOTATION::COLOR::WHITE].pawnsMask);
        if (pawnsInRank > 1)
        {
            eval += (pawnsInRank - 1) * (-50);
        }
    }

    return eval;
}

int evaluatePawnStructureForBlack(const Board& board)
{
    auto eval = 0;

    for (auto && rank : ranks)
    {
        auto pawnsInRank = __builtin_popcountll(
                rank & board.piecesBitSets[NOTATION::COLOR::BLACK].pawnsMask);
        if (pawnsInRank > 1)
        {
            eval += (pawnsInRank - 1) * (-50);
        }
    }

    return eval;
}

int evaluatePawnStructure(const Board& board, NOTATION::COLOR::color playerOnMove)
{
    auto val = evaluatePawnStructureForWhite(board) - evaluatePawnStructureForBlack(board);
    return (playerOnMove == NOTATION::COLOR::color::white)
        ? val
        : -1 * val;
}

}  // namespace

void init(const char*) { /* Do nothing */}

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
        + evaluatePawnStructure(be.board, be.board.playerOnMove);
}
