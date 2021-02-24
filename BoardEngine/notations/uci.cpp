#include "LongAlgebraicNotation.hpp"

#include <algorithm>
#include <ostream>

#include "common.hpp"
#include <publicIf/NotationConversions.hpp>

namespace notations
{
namespace uci
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

    sourcePosition = NotationConversions::getFieldNum(moveStr.substr(position, 2).c_str());
    sourcePiece = board.getField(sourcePosition);
    position += 2;
    targetPosition = NotationConversions::getFieldNum(moveStr.substr(position, 2).c_str());
    targetPiece = board.getField(targetPosition);
    position += 2;

    if ((sourcePiece & NOTATION::PIECES::PIECES_MASK) == NOTATION::PIECES::KING)
        flags |= ExtendedMove::kingMoveMask;


    if ((sourcePiece & NOTATION::PIECES::PIECES_MASK) == NOTATION::PIECES::PAWN)
    {
        flags |= ExtendedMove::pawnMoveMask;
        if (NotationConversions::getColumnNum(sourcePosition) != NotationConversions::getColumnNum(targetPosition))
            flags |= ExtendedMove::beatingMask;
    }

    if (targetPiece != 0)
        flags |= ExtendedMove::beatingMask;

    if (moveStr.size() > position)
    {
        promoteTo = createPiece(std::toupper(moveStr[++position]), playerOnMove);
        flags |= ExtendedMove::promotionMask;
    }

    return ExtendedMove{sourcePosition, targetPosition, flags, promoteTo, sourcePiece, targetPiece};
}

std::vector<char> createMoveStr(const ExtendedMove& move)
{
    std::vector<char> out;

    out.push_back('a' + NotationConversions::getColumnNum(move.source));
    out.push_back('1' + NotationConversions::getRow(move.source));

    out.push_back('a' + NotationConversions::getColumnNum(move.destination));
    out.push_back('1' + NotationConversions::getRow(move.destination));

    if (move.flags & ExtendedMove::promotionMask)
    {
        out.push_back(std::tolower(pieceToLiteral(move.promoting)));
    }

    out.push_back('\0');
    return out;
}

}  // namespace uci
}  // namespace notations

