#pragma once

#include <publicIf/Board.hpp>
#include <publicIf/Notation.hpp>

namespace materialEvaluator
{

template <typename COEF>
int evaluate(const Board& board, NOTATION::COLOR::color playerOnMove)
{
    int value = 0;

    value += (__builtin_popcountll(board.piecesBitSets[0].pawnsMask)
              - __builtin_popcountll(board.piecesBitSets[1].pawnsMask)) * COEF::pawnValue;
    value += (__builtin_popcountll(board.piecesBitSets[0].knightsMask)
              - __builtin_popcountll(board.piecesBitSets[1].knightsMask)) * COEF::knightValue;
    value += (__builtin_popcountll(board.piecesBitSets[0].bishopsMask)
              - __builtin_popcountll(board.piecesBitSets[1].bishopsMask)) * COEF::bishopValue;
    value += (__builtin_popcountll(board.piecesBitSets[0].rocksMask)
              - __builtin_popcountll(board.piecesBitSets[1].rocksMask)) * COEF::rockValue;
    value += (__builtin_popcountll(board.piecesBitSets[0].queensMask)
              - __builtin_popcountll(board.piecesBitSets[1].queensMask)) * COEF::queenValue;
    // no need to evaluate kings. It do not change the value of evaluation

    int whiteBishops = __builtin_popcountll(board.piecesBitSets[0].bishopsMask);
    int blackBishops = __builtin_popcountll(board.piecesBitSets[1].bishopsMask);

    if (whiteBishops == 2) value += COEF::dualBishopValue;
    if (blackBishops == 2) value -= COEF::dualBishopValue;

    if (__builtin_popcountll(board.piecesBitSets[0].pawnsMask) == 0)
        value += COEF::allPawnPenality;
    if (__builtin_popcountll(board.piecesBitSets[1].pawnsMask) == 0)
        value -= COEF::allPawnPenality;
    if (playerOnMove == NOTATION::COLOR::color::black)
        return -1 * value;
    return value;
}

}