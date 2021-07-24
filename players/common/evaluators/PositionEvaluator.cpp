#include "PositionEvaluator.hpp"
#include <BoardEngine.hpp>
#include "evaluationConstants.h"

#include <publicIf/NotationConversions.hpp>
#include <common/evaluators/MoveCountEvaluator.hpp>
#include <common/evaluators/MaterialEvaluator.hpp>
#include <common/evaluators/PawnStructureEvaluator.hpp>

EvaluationSettings evaluationSettings;

void EvaluationSettings::switchOfAll()
{
    materialEvaluation = false;
    pawnStructureEvaluation = false;
    moveCountEvaluation = false;
}

constexpr int mirror[64]{
        56, 57, 58, 59, 60, 61, 62, 63,
        48, 49, 50, 51, 52, 53, 54, 55,
        40, 41, 42, 43, 44, 45, 46, 47,
        32, 33, 34, 35, 36, 37, 38, 39,
        24, 25, 26, 27, 28, 29, 30, 31,
        16, 17, 18, 19, 20, 21, 22, 23,
        8, 9, 10, 11, 12, 13, 14, 15,
        0, 1, 2, 3, 4, 5, 6, 7
};

template <typename COEF>
int evaluateTable(const BoardEngine& be)
{
    auto value = 0;
    auto whitePawns = be.board.piecesBitSets[0].pawnsMask;
    while (whitePawns)
    {
        auto index = 63 - __builtin_clzll(whitePawns);
        auto bitmap = 1ull << index;

        value += COEF::pawnTable[index];
        whitePawns ^= bitmap;
    }

    auto blackPawns = be.board.piecesBitSets[1].pawnsMask;
    while (blackPawns)
    {
        auto index = 63 - __builtin_clzll(blackPawns);
        auto bitmap = 1ull << index;

        value -= COEF::pawnTable[mirror[index]];
        blackPawns ^= bitmap;
    }

    auto whiteBishops = be.board.piecesBitSets[0].bishopsMask;
    while (whiteBishops)
    {
        auto index = 63 - __builtin_clzll(whiteBishops);
        auto bitmap = 1ull << index;

        value += COEF::bishopsTable[index];
        whiteBishops ^= bitmap;
    }

    auto blackBishops = be.board.piecesBitSets[1].bishopsMask;
    while (blackBishops)
    {
        auto index = 63 - __builtin_clzll(blackBishops);
        auto bitmap = 1ull << index;

        value -= COEF::bishopsTable[mirror[index]];
        blackBishops ^= bitmap;
    }

    auto whiteKnights = be.board.piecesBitSets[0].knightsMask;
    while (whiteKnights)
    {
        auto index = 63 - __builtin_clzll(whiteKnights);
        auto bitmap = 1ull << index;

        value += COEF::knightTable[index];
        whiteKnights ^= bitmap;
    }

    auto blackKnights = be.board.piecesBitSets[1].knightsMask;
    while (blackKnights)
    {
        auto index = 63 - __builtin_clzll(blackKnights);
        auto bitmap = 1ull << index;

        value -= COEF::knightTable[mirror[index]];
        blackKnights ^= bitmap;
    }

    auto whiteRocks = be.board.piecesBitSets[0].rocksMask;
    while (whiteRocks)
    {
        auto index = 63 - __builtin_clzll(whiteRocks);
        auto bitmap = 1ull << index;

        value += COEF::rockTable[index];
        whiteRocks ^= bitmap;
    }

    auto blackRocks = be.board.piecesBitSets[1].rocksMask;
    while (blackRocks)
    {
        auto index = 63 - __builtin_clzll(blackRocks);
        auto bitmap = 1ull << index;

        value -= COEF::rockTable[mirror[index]];
        blackRocks ^= bitmap;
    }

    auto whiteQueens = be.board.piecesBitSets[0].queensMask;
    while (whiteQueens)
    {
        auto index = 63 - __builtin_clzll(whiteQueens);
        auto bitmap = 1ull << index;

        value += COEF::queensTable[index];
        whiteQueens ^= bitmap;
    }

    auto blackQueens = be.board.piecesBitSets[1].queensMask;
    while (blackQueens)
    {
        auto index = 63 - __builtin_clzll(blackQueens);
        auto bitmap = 1ull << index;

        value -= COEF::queensTable[mirror[index]];
        blackQueens ^= bitmap;
    }

    auto whiteKings = be.board.piecesBitSets[0].kingsMask;
    while (whiteKings)
    {
        auto index = 63 - __builtin_clzll(whiteKings);
        auto bitmap = 1ull << index;

        value += COEF::kingsTable[index];
        whiteKings ^= bitmap;
    }

    auto blackKings = be.board.piecesBitSets[1].kingsMask;
    while (blackKings)
    {
        auto index = 63 - __builtin_clzll(blackKings);
        auto bitmap = 1ull << index;

        value -= COEF::kingsTable[mirror[index]];
        blackKings ^= bitmap;
    }

    if (be.board.playerOnMove == NOTATION::COLOR::color::black)
        return value * -1;
    return value;
};

template <typename COEF>
int evaluateFunctionImpl(BoardEngine& be, unsigned int validMovesCount)
{
    auto gameResult = be.getREsultWithoutRepeatitionCheck(validMovesCount);
    if ((gameResult == Result::whiteWon) | (gameResult == Result::blackWon)) {
        return -10000000;
    }

    int evaluationValue = 0;
    if (evaluationSettings.materialEvaluation) {
        evaluationValue += materialEvaluator::evaluate<COEF>(be.board, be.board.playerOnMove);
    }
    if (evaluationSettings.moveCountEvaluation) {
        auto oponentValidMoves = be.generateValidMoveCount(be.board.playerOnMove + 1);
        evaluationValue += moveCountEvaluator::evaluate<COEF>(validMovesCount, oponentValidMoves);
    }
    if (evaluationSettings.pawnStructureEvaluation) {
        evaluationValue += pawnStructureEvaluator::evaluate<COEF>(be.board, be.board.playerOnMove);
    }
    evaluationValue += evaluateTable<COEF>(be);
    return evaluationValue;
}

int evaluateFunction(BoardEngine& be, unsigned int validMovesCount)
{
    return evaluateFunctionImpl<SingleEval>(be, validMovesCount);
/*    auto phase = 0;

    const auto bishop_value = 1;
    const auto knight_value = 1;
    const auto rock_value = 2;
    const auto queen_value = 4;
    const auto total = 4 * (bishop_value + knight_value + rock_value) + 2 * queen_value;

    phase = total;
    phase -= rock_value * (__builtin_popcountll(be.board.piecesBitSets[0].rocksMask)
            + __builtin_popcountll(be.board.piecesBitSets[1].rocksMask));
    phase -= knight_value * (__builtin_popcountll(be.board.piecesBitSets[0].knightsMask)
                           + __builtin_popcountll(be.board.piecesBitSets[1].knightsMask));
    phase -= bishop_value * (__builtin_popcountll(be.board.piecesBitSets[0].bishopsMask)
                           + __builtin_popcountll(be.board.piecesBitSets[1].bishopsMask));
    phase -= queen_value * (__builtin_popcountll(be.board.piecesBitSets[0].queensMask)
                             + __builtin_popcountll(be.board.piecesBitSets[1].queensMask));

    phase = (phase * 256 + (total / 2)) / total;

    return ((256 - phase) * evaluateFunctionImpl<Middle>(be, validMovesCount) +
            phase * evaluateFunctionImpl<End>(be, validMovesCount) ) / 256;*/
}