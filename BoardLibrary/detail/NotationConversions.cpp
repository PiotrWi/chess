#include <publicIf/NotationConversions.hpp>

namespace NotationConversions
{

unsigned char getPieceType(unsigned char piece)
{
	return NOTATION::PIECES::PIECES_MASK & piece;
}

NOTATION::COLOR::color getPieceColor(unsigned char piece)
{
	return static_cast<NOTATION::COLOR::color >(piece & NOTATION::COLOR::COLOR_MASK);
}

bool isRowInBoard(unsigned char row)
{
    return !(row & 0b11111000)/*row < 8u*/;
}

bool isWithCorrectRow(unsigned char position)
{
    return (0b11000000 & position) == 0u;
}

bool isColumnInBoard(unsigned char col)
{
    return !(col & 0b11111000)/*col < 8u*/;
}

}

NOTATION::COLOR::color operator++(NOTATION::COLOR::color& c)
{
    c = NotationConversions::switchColor(c);
    return c;
}

NOTATION::COLOR::color operator++(NOTATION::COLOR::color& c, int)
{
    auto tmp = c;
    c = ++c;
    return tmp;
}

bool operator==(const unsigned char uc, const NOTATION::COLOR::color c)
{
    return uc == static_cast<unsigned char>(c);
}
