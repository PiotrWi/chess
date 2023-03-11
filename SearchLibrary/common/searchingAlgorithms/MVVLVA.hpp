#pragma once

#include <core/Board.hpp>
#include "MVVLVA.hpp"
#include <common/Constants.hpp>

class MVVLVA_Comparator // Most valuable victim less valuable aggressor
{
public:
    static constexpr int weights[7][7]{
            // NOTHING  // PAWN,    ROCK,   KNIGHT,     BISHOP,     QUEEN,  KING
            {0,         0,          0,      0,          0,          0,      0}, //NOTHING
            {0,         106,        506,    306,        306,        906,    5000}, //PAWN
            {0,         101,        501,    301,        301,        901,    5000}, //ROCK
            {0,         105,        505,    305,        305,        905,    5000}, //KNIGHT
            {0,         104,        504,    304,        304,        904,    5000}, //BISHOP
            {0,         100,        500,    300,        300,        900,    5000}, //QUEEN
            {0,         107,        507,    307,        307,        907,    5000}, //KING
    };
    static bool compare(const ExtendedMove& lhs, const ExtendedMove& rhs);
};

inline bool MVVLVA_Comparator::compare(const ExtendedMove& lhs, const ExtendedMove& rhs)
{
    auto lhsVal = (lhs.flags & ExtendedMove::promotionMask) ? FIGURES_VALUE::QUEEN : 0;
    lhsVal += weights[lhs.sourcePiece >> 1][lhs.targetPiece >> 1];

    auto rhsVal = (rhs.flags & ExtendedMove::promotionMask) ? FIGURES_VALUE::QUEEN : 0;
    rhsVal += weights[rhs.sourcePiece >> 1][rhs.targetPiece >> 1];

    return lhsVal > rhsVal;
}