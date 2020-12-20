#include "publicIf/Board.hpp"

#include <algorithm>
#include <cstdint>
#include <ostream>
#include <publicIf/NotationConversions.hpp>

Move::Move()
	: source(0)
	, destination(0)
	, isPromoted(false)
	, promoteTo(0) {}

Move::Move(unsigned char s, unsigned char d, bool isPromoted, unsigned char promoteTo)
	: source(s)
	, destination(d)
	, isPromoted(isPromoted)
	, promoteTo(promoteTo) {}

Move::Move(const char* sourceStr, const char* destinationStr, bool isPromoted, unsigned char promoteTo)
	: source(NotationConversions::getFieldNum(sourceStr))
	, destination(NotationConversions::getFieldNum(destinationStr))
	, isPromoted(isPromoted)
	, promoteTo(promoteTo) {}

std::ostream& operator<<(std::ostream& os, const Move& m)
{
	char out[6];
	out[0] = 'a' + NotationConversions::getColumnNum(m.source);
	out[1] = '1' + NotationConversions::getRow(m.source);
	out[2] = '-';
	out[3] = 'a' + NotationConversions::getColumnNum(m.destination);
	out[4] = '1' + NotationConversions::getRow(m.destination);
	out[5] = '\0';
	os << out;
	return os;
}

bool operator==(const Move& lfs, const Move& rhs)
{
	return *reinterpret_cast<const uint32_t*>(&lfs) == *reinterpret_cast<const uint32_t*>(&rhs);
}

unsigned char& Board::operator[](const char* field)
{
	return fields[NotationConversions::getFieldNum(field)];
}

unsigned char& Board::operator[](const unsigned char field)
{
	return fields[field];
}

const unsigned char& Board::operator[](const char* field) const
{
	return const_cast<Board&>(*this)[field];
}

const unsigned char& Board::operator[](const unsigned char field) const
{
	return const_cast<Board&>(*this)[field];
}

bool operator==(const Board& lhs, const Board& rhs)
{
    return lhs.lastMove == rhs.lastMove and lhs.playerOnMove == rhs.playerOnMove
        and std::equal(std::begin(lhs.fields), std::end(lhs.fields), std::begin(rhs.fields));
}

void initDefault(Board& board)
{
        board["a1"] = NOTATION::COLOR::WHITE | NOTATION::PIECES::ROCK;
        board["b1"] = NOTATION::COLOR::WHITE | NOTATION::PIECES::KNIGHT;
        board["c1"] = NOTATION::COLOR::WHITE | NOTATION::PIECES::BISHOP;
        board["d1"] = NOTATION::COLOR::WHITE | NOTATION::PIECES::QUEEN;
        board["e1"] = NOTATION::COLOR::WHITE | NOTATION::PIECES::KING;
        board["f1"] = NOTATION::COLOR::WHITE | NOTATION::PIECES::BISHOP;
        board["g1"] = NOTATION::COLOR::WHITE | NOTATION::PIECES::KNIGHT;
        board["h1"] = NOTATION::COLOR::WHITE | NOTATION::PIECES::ROCK;

        char field[3] = "a2";
        for (auto&& c: {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'})
        {
                field[0] = c;
                board[field] = NOTATION::COLOR::WHITE | NOTATION::PIECES::PAWN;
        }

        board["a8"] = NOTATION::COLOR::BLACK | NOTATION::PIECES::ROCK;
        board["b8"] = NOTATION::COLOR::BLACK | NOTATION::PIECES::KNIGHT;
        board["c8"] = NOTATION::COLOR::BLACK | NOTATION::PIECES::BISHOP;
        board["d8"] = NOTATION::COLOR::BLACK | NOTATION::PIECES::QUEEN;
        board["e8"] = NOTATION::COLOR::BLACK | NOTATION::PIECES::KING;
        board["f8"] = NOTATION::COLOR::BLACK | NOTATION::PIECES::BISHOP;
        board["g8"] = NOTATION::COLOR::BLACK | NOTATION::PIECES::KNIGHT;
        board["h8"] = NOTATION::COLOR::BLACK | NOTATION::PIECES::ROCK;

        field[2] = 7;
        for (auto&& c: {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'})
        {
                field[0] = c;
                board[field] = NOTATION::COLOR::BLACK | NOTATION::PIECES::PAWN;
        }
}
