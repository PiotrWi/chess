#include <NotationConversions.hpp>

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
unsigned getFieldNum(const unsigned char row, const unsigned char col)
{
	return (row << 3u) | col;
}
