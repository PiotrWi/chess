#include "MaterialEvaluator.hpp"

#include <publicIf/Board.hpp>

static int mapToValue(unsigned char field)
{
    using namespace NOTATION::PIECES;
    using namespace NOTATION::COLOR;
    switch (field)
    {
        case WHITE | PAWN: return 100;
        case WHITE | KNIGHT: return 300;
        case WHITE | BISHOP: return 300;
        case WHITE | ROCK: return 500;
        case WHITE | QUEEN: return 900;
        case WHITE | KING: return  300;
        case BLACK | PAWN: return -100;
        case BLACK | KNIGHT: return -300;
        case BLACK | BISHOP: return -300;
        case BLACK | ROCK: return -500;
        case BLACK | QUEEN: return -900;
        case BLACK | KING: return  -300;
    }
    return  0;
}

namespace materialEvaluator
{
int evaluate(const Board &board, NOTATION::COLOR::color c)
{
    int value = 0;
    for (const auto field : board.fields)
    {
        value += mapToValue(field);
    }
    if (c == NOTATION::COLOR::color::black)
        return -1 * value;
    return value;
}
}