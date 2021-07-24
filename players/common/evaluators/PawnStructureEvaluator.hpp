#pragma once

#include <utility>

#include <publicIf/Board.hpp>
#include <publicIf/Notation.hpp>

#include <publicIf/Board.hpp>
#include <publicIf/NotationConversions.hpp>
#include <detail/bitboardslookups.hpp>

#include <publicIf/Notation.hpp>

namespace pawnStructureEvaluator
{

struct PassedPawns
{
    int rank7 = 0;
    int rank6 = 0;
    int rank5 = 0;
    int rank4 = 0;
    int rank3 = 0;
    int rank2 = 0;
};

namespace
{
    int evaluateDoubledPawnsForWhite(const Board & board)
    {
        auto eval = 0;

        for (auto && rank : ranks)
        {
            auto pawnsInRank = __builtin_popcountll(
                    rank & board.piecesBitSets[NOTATION::COLOR::WHITE].pawnsMask);
            if (pawnsInRank > 1)
            {
                eval += (pawnsInRank - 1);
            }
        }

        return eval;
    }

    int evaluateDoubledPawnsForBlack(const Board& board)
    {
        auto eval = 0;

        for (auto && rank : ranks)
        {
            auto pawnsInRank = __builtin_popcountll(
                    rank & board.piecesBitSets[NOTATION::COLOR::BLACK].pawnsMask);
            if (pawnsInRank > 1)
            {
                eval += (pawnsInRank - 1);
            }
        }

        return eval;
    }

}  // namespace



inline bool isWhitePassed(uint64_t pawnsBitMask, const Board& board)
{
    auto frontier = pawnsBitMask >> 8 | (pawnsBitMask& NOT_A_COL) << 7 | (pawnsBitMask& NOT_H_COL) << 9;
    frontier |= frontier << 8;
    frontier |= frontier << 16;
    frontier |= frontier << 16;

    return (frontier & board.piecesBitSets[NOTATION::COLOR::BLACK].pawnsMask) == 0;
}

inline std::pair<int, PassedPawns> evaluateIsolatedPawnsForWhite(const Board& board)
{
    int isolatedPawns = 0;
    PassedPawns passedPawns;

    uint64_t mostAdvancedPawns = board.piecesBitSets[NOTATION::COLOR::WHITE].pawnsMask;
    uint64_t lessAdvancedPawns = 0;
    lessAdvancedPawns |= (0x00'ff'00'00'00'00'00'00ul & mostAdvancedPawns) >> 8;
    lessAdvancedPawns |= (0x00'00'ff'00'00'00'00'00ul & (mostAdvancedPawns | lessAdvancedPawns)) >> 8;
    lessAdvancedPawns |= (0x00'00'00'ff'00'00'00'00ul & (mostAdvancedPawns | lessAdvancedPawns)) >> 8;
    lessAdvancedPawns |= (0x00'00'00'00'ff'00'00'00ul & (mostAdvancedPawns | lessAdvancedPawns)) >> 8;
    lessAdvancedPawns |= (0x00'00'00'00'00'ff'00'00ul & (mostAdvancedPawns | lessAdvancedPawns)) >> 8;

    mostAdvancedPawns ^= (lessAdvancedPawns & mostAdvancedPawns);
    while (mostAdvancedPawns)
    {
        auto pawnsIndex = 63 - __builtin_clzll(mostAdvancedPawns);
        auto pawnsBitMask = 1ull << pawnsIndex;

        auto column = NotationConversions::getColumnNum(pawnsIndex);
        uint64_t bitMask = ((ranks[column] & NOT_A_COL) >> 1) | ((ranks[column] & NOT_H_COL) << 1);
        if ((bitMask & board.piecesBitSets[NOTATION::COLOR::WHITE].pawnsMask) == 0)
        {
            isolatedPawns += 1;
        }
        if (isWhitePassed(pawnsBitMask, board))
        {
            auto row = NotationConversions::getRow(pawnsIndex);
            if (row == 6) passedPawns.rank7 += 1;
            if (row == 5) passedPawns.rank6 += 1;
            if (row == 4) passedPawns.rank5 += 1;
            if (row == 3) passedPawns.rank4 += 1;
            if (row == 2) passedPawns.rank3 += 1;
            if (row == 1) passedPawns.rank2 += 1;
        }
        mostAdvancedPawns ^= pawnsBitMask;
    }
    return std::make_pair(isolatedPawns, passedPawns);
}

inline bool isBlackPassed(uint64_t pawnsBitMask, const Board& board)
{
    auto frontier = pawnsBitMask >> 8 | (pawnsBitMask& NOT_A_COL) >> 9 | (pawnsBitMask& NOT_H_COL) >> 7;
    frontier |= frontier >> 8;
    frontier |= frontier >> 16;
    frontier |= frontier >> 16;

    return (frontier & board.piecesBitSets[NOTATION::COLOR::WHITE].pawnsMask) == 0;
}

inline std::pair<int, PassedPawns> evaluateIsolatedPawnsForBlack(const Board& board)
{
    int isolatedPawns = 0;
    PassedPawns passedPawns;

    uint64_t mostAdvancedPawns = board.piecesBitSets[NOTATION::COLOR::BLACK].pawnsMask;
    uint64_t lessAdvancedPawns = 0;
    lessAdvancedPawns |= (0x00'00'00'00'00'00'ff'00ul & mostAdvancedPawns) << 8;
    lessAdvancedPawns |= (0x00'00'00'00'00'ff'00'00ul & (mostAdvancedPawns | lessAdvancedPawns)) << 8;
    lessAdvancedPawns |= (0x00'00'00'00'ff'00'00'00ul & (mostAdvancedPawns | lessAdvancedPawns)) << 8;
    lessAdvancedPawns |= (0x00'00'00'ff'00'00'00'00ul & (mostAdvancedPawns | lessAdvancedPawns)) << 8;
    lessAdvancedPawns |= (0x00'00'ff'00'00'00'00'00ul & (mostAdvancedPawns | lessAdvancedPawns)) << 8;

    mostAdvancedPawns ^= (lessAdvancedPawns & mostAdvancedPawns);
    while (mostAdvancedPawns)
    {
        auto pawnsIndex = 63 - __builtin_clzll(mostAdvancedPawns);
        auto pawnsBitMask = 1ull << pawnsIndex;

        auto column = NotationConversions::getColumnNum(pawnsIndex);
        uint64_t bitMask = ((ranks[column] & NOT_A_COL) >> 1) | ((ranks[column] & NOT_H_COL) << 1);
        if ((bitMask & board.piecesBitSets[NOTATION::COLOR::BLACK].pawnsMask) == 0)
        {
            isolatedPawns += 1;
        }
        if (isBlackPassed(pawnsBitMask, board))
        {
            auto row = NotationConversions::getRow(pawnsIndex);
            if (row == 1) passedPawns.rank7 += 1;
            if (row == 2) passedPawns.rank6 += 1;
            if (row == 3) passedPawns.rank5 += 1;
            if (row == 4) passedPawns.rank4 += 1;
            if (row == 5) passedPawns.rank3 += 1;
            if (row == 6) passedPawns.rank2 += 1;
        }
        mostAdvancedPawns ^= pawnsBitMask;
    }
    return std::make_pair(isolatedPawns, passedPawns);
}

template <typename COEF>
int evaluate(const Board& board, NOTATION::COLOR::color playerOnMove)
{
    auto doubledPawns = COEF::doubledPawn * (evaluateDoubledPawnsForWhite(board) - evaluateDoubledPawnsForBlack(board));
    auto isolatedAndPassedWhite = evaluateIsolatedPawnsForWhite(board);
    auto isolatedAndPassedBlack = evaluateIsolatedPawnsForBlack(board);
    auto isolatedPawns = COEF::isolatedPAwn * isolatedAndPassedWhite.first - isolatedAndPassedBlack.first;
    auto rank7passed = COEF::passedRank7 * (isolatedAndPassedWhite.second.rank7 - isolatedAndPassedBlack.second.rank7);
    auto rank6passed = COEF::passedRank6 * (isolatedAndPassedWhite.second.rank6 - isolatedAndPassedBlack.second.rank6);
    auto rank5passed = COEF::passedRank5 * (isolatedAndPassedWhite.second.rank5 - isolatedAndPassedBlack.second.rank5);
    auto rank4passed = COEF::passedRank4 * (isolatedAndPassedWhite.second.rank4 - isolatedAndPassedBlack.second.rank4);
    auto rank3passed = COEF::passedRank3 * (isolatedAndPassedWhite.second.rank3 - isolatedAndPassedBlack.second.rank3);
    auto rank2passed = COEF::passedRank2 * (isolatedAndPassedWhite.second.rank2 - isolatedAndPassedBlack.second.rank2);

    auto val = doubledPawns +
               isolatedPawns +
               rank7passed +
               rank6passed +
               rank5passed +
               rank4passed +
               rank3passed +
               rank2passed;
    return (playerOnMove == NOTATION::COLOR::color::white)
           ? val
           : -1 * val;
}


}