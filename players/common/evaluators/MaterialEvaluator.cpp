#include "MaterialEvaluator.hpp"

#include <publicIf/Board.hpp>

constexpr int pawnValue = 100;
constexpr int knightValue = 300;
constexpr int bishopValue = 300;
constexpr int rockValue = 500;
constexpr int queenValue = 900;
constexpr int kingValue = 300;

namespace materialEvaluator
{

int evaluate(const Board &board, NOTATION::COLOR::color playerOnMove)
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
    value += (__builtin_popcountll(board.piecesBitSets[0].kingsMask)
            - __builtin_popcountll(board.piecesBitSets[1].kingsMask)) * kingValue;

    if (playerOnMove == NOTATION::COLOR::color::black)
        return -1 * value;
    return value;
}

}  // namespace materialEvaluator

