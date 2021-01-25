#pragma once

#include <publicIf/Notation.hpp>

namespace FIGURES_VALUE
{
    constexpr int PAWN = 100;
    constexpr int KNIGHT = 300;
    constexpr int BISHOP = 300;
    constexpr int ROCK = 500;
    constexpr int QUEEN = 900;
    constexpr int KING = 300;
}

constexpr int mapFigureToValue(unsigned char figure)
{
    switch (figure)
    {
        case NOTATION::PIECES::PAWN: return FIGURES_VALUE::PAWN;
        case NOTATION::PIECES::KNIGHT: return FIGURES_VALUE::KNIGHT;
        case NOTATION::PIECES::BISHOP: return FIGURES_VALUE::BISHOP;
        case NOTATION::PIECES::ROCK: return FIGURES_VALUE::ROCK;
        case NOTATION::PIECES::QUEEN: return FIGURES_VALUE::QUEEN;
        case NOTATION::PIECES::KING: return FIGURES_VALUE::KING;
    }

#ifdef ASSERTSON
    assert(0);
#endif

    return 0;
}