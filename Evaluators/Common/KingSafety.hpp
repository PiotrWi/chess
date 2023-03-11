#pragma once

#include "core/Board.hpp"
#include "core/NotationConversions.hpp"

namespace king_safety
{

bool isEndGame(const Board& board)
{
    return not (board.piecesBitSets[NOTATION::COLOR::WHITE].queensMask
        | board.piecesBitSets[NOTATION::COLOR::BLACK].queensMask);
}

struct KingSafetyCoefficients
{
    int ShortCastleFullStructure = 20;
    int ShortCastleFullLooftStructure = 20;
    int ShortCastleSmallStructure = 30;
    int ShortCastleSmallLooftStructure = 20;
};

template <NOTATION::COLOR::color TColor>
int evaluate(const Board& board, KingSafetyCoefficients coeff);

template <>
int evaluate<NOTATION::COLOR::color::white>(const Board& board, KingSafetyCoefficients coeff)
{
    int eval = 0;
    constexpr uint64_t ShortCastleKingPattern = "g1"_bit| "h1"_bit;

    constexpr uint64_t ShortCastleFullPawnPattern = "f2"_bit | "g2"_bit| "h2"_bit;
    constexpr uint64_t ShortCastleFullLooftPawnPattern = "f2"_bit | "g2"_bit| "h3"_bit;
    constexpr uint64_t ShortCastleSmallPawnPattern = "g2"_bit| "h2"_bit;
    constexpr uint64_t ShortCastleSmallLooftPawnPattern = "g2"_bit| "h3"_bit;

    if (board.piecesBitSets[NOTATION::COLOR::WHITE].kingsMask & ShortCastleKingPattern)
    {
        const auto& pawns = board.piecesBitSets[NOTATION::COLOR::WHITE].pawnsMask;
        if ((pawns & ShortCastleFullPawnPattern) == ShortCastleFullPawnPattern)
        {
            eval += coeff.ShortCastleFullStructure;
        }
        if ((pawns & ShortCastleFullLooftPawnPattern) == ShortCastleFullLooftPawnPattern)
        {
            eval += coeff.ShortCastleFullLooftStructure;
        }
        if ((pawns & ShortCastleSmallPawnPattern) == ShortCastleSmallPawnPattern)
        {
            eval += coeff.ShortCastleSmallStructure;
        }
        if ((pawns & ShortCastleSmallLooftPawnPattern) == ShortCastleSmallLooftPawnPattern)
        {
            eval += coeff.ShortCastleSmallLooftStructure;
        }
    }

    return eval;
}

template <>
int evaluate<NOTATION::COLOR::color::black>(const Board& board, KingSafetyCoefficients coeff)
{
    int eval = 0;
    constexpr uint64_t ShortCastleKingPattern = "g8"_bit| "h8"_bit;

    constexpr uint64_t ShortCastleFullPawnPattern = "f7"_bit | "g7"_bit| "h7"_bit;
    constexpr uint64_t ShortCastleFullLooftPawnPattern = "f7"_bit | "g7"_bit| "h6"_bit;
    constexpr uint64_t ShortCastleSmallPawnPattern = "g7"_bit| "h7"_bit;
    constexpr uint64_t ShortCastleSmallLooftPawnPattern = "g7"_bit| "h6"_bit;

    if (board.piecesBitSets[NOTATION::COLOR::BLACK].kingsMask & ShortCastleKingPattern)
    {
        const auto& pawns = board.piecesBitSets[NOTATION::COLOR::BLACK].pawnsMask;
        if ((pawns & ShortCastleFullPawnPattern) == ShortCastleFullPawnPattern)
        {
            eval += coeff.ShortCastleFullStructure;
        }
        if ((pawns & ShortCastleFullLooftPawnPattern) == ShortCastleFullLooftPawnPattern)
        {
            eval += coeff.ShortCastleFullLooftStructure;
        }
        if ((pawns & ShortCastleSmallPawnPattern) == ShortCastleSmallPawnPattern)
        {
            eval += coeff.ShortCastleSmallStructure;
        }
        if ((pawns & ShortCastleSmallLooftPawnPattern) == ShortCastleSmallLooftPawnPattern)
        {
            eval += coeff.ShortCastleSmallLooftStructure;
        }
    }

    return eval;
}

int evaluate(const Board& board, NOTATION::COLOR::color playerOnMove, KingSafetyCoefficients coeff)
{
    if (isEndGame(board))
    {
        return 0;
    }
    auto val = evaluate<NOTATION::COLOR::color::white>(board, coeff)
            - evaluate<NOTATION::COLOR::color::black>(board, coeff);
    return (playerOnMove == NOTATION::COLOR::color::white)
           ? val
           : -1 * val;
}

}  // namespace king_safety
