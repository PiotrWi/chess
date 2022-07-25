#pragma once

#include <boost/serialization/strong_typedef.hpp>
#include <tuple>

#include <publicIf/Notation.hpp>
#include <publicIf/NotationConversions.hpp>

BOOST_STRONG_TYPEDEF(int, TPawnValue)
BOOST_STRONG_TYPEDEF(int, TBishopValue)
BOOST_STRONG_TYPEDEF(int, TKnightValue)
BOOST_STRONG_TYPEDEF(int, TRockValue)
BOOST_STRONG_TYPEDEF(int, TQueenValue)

BOOST_STRONG_TYPEDEF(int, TPawnCount)
BOOST_STRONG_TYPEDEF(int, TBishopCount)
BOOST_STRONG_TYPEDEF(int, TKnightCount)
BOOST_STRONG_TYPEDEF(int, TRockCount)
BOOST_STRONG_TYPEDEF(int, TQueenCount)

namespace matherial_evaluator
{

using RESULT_TYPE = std::tuple<TPawnCount, TBishopCount, TKnightCount, TRockCount, TQueenCount>;
using VALUES_TYPE = std::tuple<TPawnValue, TBishopValue, TKnightValue, TRockValue, TQueenValue>;

RESULT_TYPE get(const Board &board)
{
    using index_type = decltype(NOTATION::COLOR::WHITE);
    constexpr index_type WHITE = NOTATION::COLOR::WHITE;
    constexpr index_type BLACK = NOTATION::COLOR::BLACK;
    return RESULT_TYPE(
        TPawnCount(__builtin_popcountll(board.piecesBitSets[WHITE].pawnsMask) - __builtin_popcountll(board.piecesBitSets[BLACK].pawnsMask)),
        TBishopCount(__builtin_popcountll(board.piecesBitSets[WHITE].bishopsMask) - __builtin_popcountll(board.piecesBitSets[BLACK].bishopsMask)),
        TKnightCount(__builtin_popcountll(board.piecesBitSets[WHITE].knightsMask) - __builtin_popcountll(board.piecesBitSets[BLACK].knightsMask)),
        TRockCount(__builtin_popcountll(board.piecesBitSets[WHITE].rocksMask) - __builtin_popcountll(board.piecesBitSets[BLACK].rocksMask)),
        TQueenCount(__builtin_popcountll(board.piecesBitSets[WHITE].queensMask) - __builtin_popcountll(board.piecesBitSets[BLACK].queensMask)));

}

int evaluate(const Board &board, NOTATION::COLOR::color playerOnMove, VALUES_TYPE piecesValues)
{
    auto matherialDiff = matherial_evaluator::get(board);
    int value = 0;

    value += std::get<TPawnCount>(matherialDiff) * std::get<TPawnValue>(piecesValues);
    value += std::get<TBishopCount>(matherialDiff) * std::get<TBishopValue>(piecesValues);
    value += std::get<TKnightCount>(matherialDiff) * std::get<TKnightValue>(piecesValues);
    value += std::get<TRockCount>(matherialDiff) * std::get<TRockValue>(piecesValues);
    value += std::get<TQueenCount>(matherialDiff) * std::get<TQueenValue>(piecesValues);

    if (playerOnMove == NOTATION::COLOR::color::black)
        return -1 * value;
    return value;
}

}  // namespace matherial_evaluator
