#include "common.hpp"

namespace notations
{

unsigned char createPiece(const char pieceSign, NOTATION::COLOR::color playerOnMove)
{
    unsigned char pattern = static_cast<unsigned char>(playerOnMove);
    switch (pieceSign)
    {
    case 'Q':
        return NOTATION::PIECES::QUEEN | pattern;
    case 'B':
        return NOTATION::PIECES::BISHOP | pattern;
    case 'R':
        return NOTATION::PIECES::ROCK | pattern;
    case 'N':
        return NOTATION::PIECES::KNIGHT | pattern;
    case 'K':
        return NOTATION::PIECES::KING | pattern;
    }
    return 0;
}

char pieceToLiteral(unsigned char field)
{
    unsigned char piece = field & NOTATION::PIECES::PIECES_MASK;
    switch (piece)
    {
        case NOTATION::PIECES::QUEEN: return 'Q';
        case NOTATION::PIECES::BISHOP: return 'B';
        case NOTATION::PIECES::ROCK: return 'R';
        case NOTATION::PIECES::KNIGHT: return 'N';
        case NOTATION::PIECES::KING: return 'K';
    }
    return ' ';
}

}
