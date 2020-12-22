#include "LongAlgebraicNotation.hpp"
#include <publicIf/NotationConversions.hpp>
namespace
{

unsigned char createPiece(const char pieceSign, NOTATION::COLOR::color playerOnMove)
{
	unsigned char pattern = static_cast<unsigned char>(playerOnMove) | NOTATION::MOVED::MOVED_MASK;
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
    }
    return ' ';
}

}
Move createMove (const std::string& moveStr,
	NOTATION::COLOR::color playerOnMove)
{
	if (moveStr == "O-O")
	{
		if (playerOnMove == NOTATION::COLOR::color::white)
		{
			return Move{"e1", "g1"};
		}
		return Move{"e8", "g8"};
	}
	if (moveStr == "O-O-O")
	{
		if (playerOnMove == NOTATION::COLOR::color::white)
		{
			return Move{"e1", "c1"};
		}
        return Move{"e8", "c8"};
	}

	auto basePos = 0u;
	if (isupper(moveStr[0]))
	{
		basePos += 1;
	}

	auto isPromoted = (moveStr.size() > 6);

	if (isPromoted)
	{
		return Move{moveStr.substr(basePos, 2).c_str()
			, moveStr.substr(basePos + 3, 2).c_str(),
			true,
			createPiece(moveStr[basePos + 6], playerOnMove)};
	}
	return Move{moveStr.substr(basePos, 2).c_str()
		, moveStr.substr(basePos + 3, 2).c_str()};
}

std::vector<char> createMoveStr(const Move& m)
{
    std::vector<char> out;
    out.reserve(6 + (m.isPromoted ? 2 : 0));

    out[0] = 'a' + NotationConversions::getColumnNum(m.source);
    out[1] = '1' + NotationConversions::getRow(m.source);
    out[2] = '-';
    out[3] = 'a' + NotationConversions::getColumnNum(m.destination);
    out[4] = '1' + NotationConversions::getRow(m.destination);

    if (m.isPromoted)
    {
        out[5] = '=';
        out[6] = pieceToLiteral(m.promoteTo);
    }
    out.back() = '\0';
    return out;
}
