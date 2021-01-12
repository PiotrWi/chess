#include "LongAlgebraicNotation.hpp"

#include <ostream>

#include <publicIf/NotationConversions.hpp>

namespace
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
    out.resize(6 + (m.isPromoted ? 2 : 0));

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

std::ostream& operator<<(std::ostream& os, const Move& m)
{
    return os << (const char*)(createMoveStr(m).data());
}


ExtendedMove createExtendedMoveFromSimpleStr (const std::string& moveStr,
    NOTATION::COLOR::color playerOnMove, const Board& board)
{
    if (moveStr == "O-O")
    {
        if (playerOnMove == NOTATION::COLOR::color::white)
        {
            return ExtendedMove::whiteShortCaste();
        }
        return ExtendedMove::blackShortCaste();
    }
    if (moveStr == "O-O-O")
    {
        if (playerOnMove == NOTATION::COLOR::color::white)
        {
            return ExtendedMove::whiteLongCaste();
        }
        return ExtendedMove::whiteLongCaste();
    }
    auto sourcePosition = NotationConversions::getFieldNum(moveStr.substr(0, 2).c_str());
    auto targetPosition = NotationConversions::getFieldNum(moveStr.substr(3, 2).c_str());

    auto isPromoted = (moveStr.size() > 6);
    unsigned char promotedTo = 0;
    unsigned char flags = 0;

    if (isPromoted)
    {
        flags |= ExtendedMove::promotionMask;
        promotedTo = createPiece(moveStr[6], playerOnMove);
    }
    if ((board[sourcePosition] & NOTATION::PIECES::PIECES_MASK) == NOTATION::PIECES::PAWN)
    {
        flags |= ExtendedMove::pawnMoveMask;
    }
    if ((board[sourcePosition] & NOTATION::PIECES::PIECES_MASK) == NOTATION::PIECES::KING)
    {
        flags |= ExtendedMove::kingMoveMask;
    }
    if (board[targetPosition] != 0)
    {
        flags |= ExtendedMove::beatingMask;
    }
    return ExtendedMove{sourcePosition, targetPosition, flags, promotedTo, board[sourcePosition], board[targetPosition]};
}

ExtendedMove createExtendedMove (const std::string& moveStr,
    NOTATION::COLOR::color playerOnMove, unsigned char targetField)
{
    if (moveStr == "O-O")
    {
        if (playerOnMove == NOTATION::COLOR::color::white)
        {
            return ExtendedMove::whiteShortCaste();
        }
        return ExtendedMove::blackShortCaste();
    }
    if (moveStr == "O-O-O")
    {
        if (playerOnMove == NOTATION::COLOR::color::white)
        {
            return ExtendedMove::whiteLongCaste();
        }
        return ExtendedMove::whiteLongCaste();
    }

    unsigned char flags = 0;
    unsigned char sourcePosition = 0;
    unsigned char sourcePiece = 0;
    unsigned char targetPosition = 0;
    unsigned char promoteTo = 0;

    unsigned char position = 0;
    if (std::isupper(moveStr[position]))
    {
        sourcePiece = createPiece(moveStr[position], playerOnMove);
        ++position;
        if ((sourcePiece & NOTATION::PIECES::PIECES_MASK) == NOTATION::PIECES::KING)
        {
            flags |= ExtendedMove::kingMoveMask;
        }
    }
    else
    {
        sourcePiece = static_cast<unsigned char>(playerOnMove) | NOTATION::PIECES::PAWN;
        flags |= ExtendedMove::pawnMoveMask;
    }

    sourcePiece = NotationConversions::getFieldNum(moveStr.substr(position, 2).c_str());
    position += 2;

    if (moveStr[position] == 'x')
    {
        flags |= ExtendedMove::beatingMask;
        ++position;
    }

    targetPosition = NotationConversions::getFieldNum(moveStr.substr(position, 2).c_str());
    position += 2;

    if (moveStr.size() > position)
    {
        promoteTo = createPiece(moveStr[++position], playerOnMove);
        flags |= ExtendedMove::promotionMask;

    }

    return ExtendedMove{sourcePosition, targetPosition, flags, promoteTo, sourcePiece, targetField};
}

std::vector<char> createMoveStr(const ExtendedMove&)
{
    std::vector<char> out;
    return {};
}

std::ostream& operator<<(std::ostream& os, const ExtendedMove&)
{
    return os;
}
