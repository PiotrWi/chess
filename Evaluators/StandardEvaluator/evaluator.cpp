#include <evaluatorIf.hpp>

#include <detail/bitboardslookups.hpp>
#include <publicIf/NotationConversions.hpp>

namespace
{

constexpr int pawnValue = 100;
constexpr int knightValue = 300;
constexpr int bishopValue = 300;
constexpr int rockValue = 500;
constexpr int queenValue = 900;
constexpr int kingValue = 300;

int evaluateMoveCount(int playerOnMoveMovesCount, int oponentMovesCount)
{
    return (playerOnMoveMovesCount - oponentMovesCount) * 10;
}

int evaluateMaterial(const Board &board, NOTATION::COLOR::color playerOnMove)
{
    int value = 0;
    value += (__builtin_popcountll(board.piecesBitSets[0].pawnsMask)
            - __builtin_popcountll(board.piecesBitSets[1].pawnsMask)) * pawnValue;
    value += (__builtin_popcountll(board.piecesBitSets[0].knightsMask)
            - __builtin_popcountll(board.piecesBitSets[1].knightsMask)) * knightValue;
    value += (__builtin_popcountll(board.piecesBitSets[0].bishopsMask)
            - __builtin_popcountll(board.piecesBitSets[1].bishopsMask)) * bishopValue;
    value += (__builtin_popcountll(board.piecesBitSets[0].rocksMask)
            - __builtin_popcountll(board.piecesBitSets[1].rocksMask)) * rockValue;
    value += (__builtin_popcountll(board.piecesBitSets[0].queensMask)
            - __builtin_popcountll(board.piecesBitSets[1].queensMask)) * queenValue;
    // no need to evaluate kings. It do not change the value of evaluation

    if (playerOnMove == NOTATION::COLOR::color::black)
        return -1 * value;
    return value;
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
    return evaluateMaterial(be.board, be.board.playerOnMove)
        + evaluateMoveCount(validMovesCount, oponentValidMoves)
        + evaluatePawnStructure(be.board, be.board.playerOnMove);
}
