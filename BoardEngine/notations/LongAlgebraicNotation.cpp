#include "LongAlgebraicNotation.hpp"

#include <algorithm>
#include <ostream>

#include "common.hpp"
#include <publicIf/NotationConversions.hpp>

namespace notations
{
namespace long_algebraic
{

ExtendedMove createExtendedMove (const std::string& moveStr,
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
        return ExtendedMove::blackLongCaste();
    }

    unsigned char flags = 0;
    unsigned char sourcePosition = 0;
    unsigned char sourcePiece = 0;
    unsigned char targetPosition = 0;
    unsigned char targetPiece = 0;
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

    sourcePosition = NotationConversions::getFieldNum(moveStr.substr(position, 2).c_str());
    position += 2;

    if (moveStr[position] == 'x')
    {
        flags |= ExtendedMove::beatingMask;
        ++position;
    }

    targetPosition = NotationConversions::getFieldNum(moveStr.substr(position, 2).c_str());
    targetPiece = board[targetPosition];

    position += 2;

    if (moveStr.size() > position)
    {
        promoteTo = createPiece(moveStr[++position], playerOnMove);
        flags |= ExtendedMove::promotionMask;

    }

    return ExtendedMove{sourcePosition, targetPosition, flags, promoteTo, sourcePiece, targetPiece};
}

std::vector<char> createMoveStr(const ExtendedMove& move)
{
    std::vector<char> out;

    if (std::any_of(std::begin(writablePieces), std::end(writablePieces), [&](auto&& writablePiece)
    {
        return (move.sourcePiece & NOTATION::PIECES::PIECES_MASK) == writablePiece;
    }))
    {
        out.push_back(pieceToLiteral(move.sourcePiece));
    }
    out.push_back('a' + NotationConversions::getColumnNum(move.source));
    out.push_back('1' + NotationConversions::getRow(move.source));

    if (move.flags & ExtendedMove::beatingMask)
    {
        out.push_back('x');
    }

    out.push_back('a' + NotationConversions::getColumnNum(move.destination));
    out.push_back('1' + NotationConversions::getRow(move.destination));

    if (move.flags & ExtendedMove::promotionMask)
    {
        out.push_back('=');
        out.push_back(pieceToLiteral(move.promoting));
    }

    out.push_back('\0');
    return out;
}

}  // namespace long_algebraic
}  // namespace notations

std::ostream& operator<<(std::ostream& os, const ExtendedMove& move)
{
    return os << (const char*)(notations::long_algebraic::createMoveStr(move).data());
}
