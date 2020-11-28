#include <NotationConversions.hpp>

namespace NotationConversions
{

unsigned char getColumnNum(unsigned char field)
{
	return field % 8u;
}

unsigned char getRow(unsigned char field)
{
	return (field >> 3u);
}

NOTATION::COLOR::color switchColor(NOTATION::COLOR::color c)
{
	auto colorNum = static_cast<unsigned char>(c);
	return NOTATION::COLOR::color(++colorNum & NOTATION::COLOR::COLOR_MASK);
}

unsigned char getFieldNum(const unsigned char row, const unsigned char col)
{
	return (row << 3u) | col;
}

unsigned char getPieceType(unsigned char piece)
{
	return NOTATION::PIECES::PIECES_MASK & piece;
}

NOTATION::COLOR::color getPieceColor(unsigned char piece)
{
	return static_cast<NOTATION::COLOR::color >(piece & NOTATION::COLOR::COLOR_MASK);
}

}
