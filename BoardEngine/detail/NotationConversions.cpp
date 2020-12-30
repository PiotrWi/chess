#include <publicIf/NotationConversions.hpp>

namespace NotationConversions
{

unsigned char getColumnNum(unsigned char field)
{
	return field & 0b111;
}

unsigned char getRow(unsigned char field)
{
	return (field >> 3u);
}

NOTATION::COLOR::color switchColor(NOTATION::COLOR::color c)
{
	auto colorNum = static_cast<unsigned char>(c);
	return NOTATION::COLOR::color(colorNum ^ NOTATION::COLOR::COLOR_MASK);
}

unsigned char getFieldNum(const unsigned char row, const unsigned char col)
{
	return (row << 3u) | col;
}

unsigned char getFieldNum(const char* field)
{
	unsigned char collumn = field[0] - 'a';
    unsigned char row = field[1] - '1';
	return row*8u + collumn;
}

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

NOTATION::COLOR::color operator+(NOTATION::COLOR::color& c, unsigned char)
{
    return NotationConversions::switchColor(c);
}
