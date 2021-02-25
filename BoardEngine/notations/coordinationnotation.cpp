#include "coordinationnotation.hpp"

#include <algorithm>
#include <ostream>

#include "common.hpp"
#include <publicIf/NotationConversions.hpp>

namespace notations
{
namespace coordinates
{
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
    auto sourcePosition = NotationConversions::getFieldNum(moveStr.substr(0, 2).c_str());
    auto targetPosition = NotationConversions::getFieldNum(moveStr.substr(3, 2).c_str());

    auto isPromoted = (moveStr.size() > 6);
    unsigned char promotedTo = 0;
    unsigned char flags = 0;

    if (board.getField(targetPosition) != 0)
    {
        flags |= ExtendedMove::beatingMask;
    }
    if ((board.getField(sourcePosition) & NOTATION::PIECES::PIECES_MASK) == NOTATION::PIECES::PAWN)
    {
        flags |= ExtendedMove::pawnMoveMask;
        if (NotationConversions::getColumnNum(sourcePosition) != NotationConversions::getColumnNum(targetPosition)
            and board.getField(targetPosition) == 0)
        {
            flags |= ExtendedMove::enPasantMask;
            flags |= ExtendedMove::beatingMask;
            return ExtendedMove(sourcePosition,
                                targetPosition,
                                flags,
                                promotedTo,
                                board.getField(sourcePosition),
                                static_cast<unsigned char>(playerOnMove+1) | NOTATION::PIECES::PAWN);
        }
    }
    if (isPromoted)
    {
        flags |= ExtendedMove::promotionMask;
        promotedTo = createPiece(moveStr[6], playerOnMove);
    }
    if ((board.getField(sourcePosition) & NOTATION::PIECES::PIECES_MASK) == NOTATION::PIECES::KING)
    {
        flags |= ExtendedMove::kingMoveMask;
    }

    return ExtendedMove{sourcePosition, targetPosition, flags, promotedTo, board.getField(sourcePosition), board.getField(targetPosition)};
}
}  // namespace coordinates
}  // namespace notations

std::ostream& operator<<(std::ostream& os, const Move& m)
{
    return os << (const char*)(notations::coordinates::createMoveStr(m).data());
}
