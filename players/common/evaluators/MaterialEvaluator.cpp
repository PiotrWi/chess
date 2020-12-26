#include "MaterialEvaluator.hpp"

#include <publicIf/Board.hpp>

static signed char mapToValue(unsigned char field)
{
    using namespace NOTATION::PIECES;
    using namespace NOTATION::COLOR;
    switch (field & NOTATION::COLOR_AND_PIECE_MASK)
    {
        case WHITE | PAWN: return 1;
        case WHITE | KNIGHT: return 3;
        case WHITE | BISHOP: return 3;
        case WHITE | ROCK: return 5;
        case WHITE | QUEEN: return 9;
        case WHITE | KING: return  3;
        case BLACK | PAWN: return -1;
        case BLACK | KNIGHT: return -3;
        case BLACK | BISHOP: return -3;
        case BLACK | ROCK: return -5;
        case BLACK | QUEEN: return -9;
        case BLACK | KING: return  -3;
    }
    return  0;
}

namespace materialEvaluator
{
signed char evaluate(const Board &board, NOTATION::COLOR::color c)
{
    signed char value = 0;
    for (const auto field : board.fields)
    {
        value += mapToValue(field);
    }
    if (c == NOTATION::COLOR::color::black)
        return -1 * value;
    return 0;
}
}