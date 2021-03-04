#include "fen.h"

#include <ctype.h>
#include <string>

#include <hashing/zobrist.hpp>
#include <BoardEngine.hpp>

namespace notations
{
namespace coordinates
{

BoardEngine initByFen(std::string fenStr)
{
    BoardEngine be;
    Board b = {};

    auto strPos = 0u;
    {
        auto chessPos = 0; // leftTop is 0 here
        while (fenStr[strPos] != ' ')
        {
            if (fenStr[strPos] == '/')
            {
                ++strPos;
                continue;
            }
            if (isdigit(fenStr[strPos]))
            {
                chessPos += fenStr[strPos] - '0';
                ++strPos;
                continue;
            }
            auto pos = chessPos % 8 + (7 - chessPos / 8) * 8;
            switch (fenStr[strPos])
            {
                case 'r':
                    b.setField(pos, NOTATION::PIECES::ROCK | NOTATION::COLOR::BLACK);
                    break;
                case 'n':
                    b.setField(pos, NOTATION::PIECES::KNIGHT | NOTATION::COLOR::BLACK);
                    break;
                case 'b':
                    b.setField(pos, NOTATION::PIECES::BISHOP | NOTATION::COLOR::BLACK);
                    break;
                case 'k':
                    b.setField(pos, NOTATION::PIECES::KING | NOTATION::COLOR::BLACK);
                    break;
                case 'q':
                    b.setField(pos, NOTATION::PIECES::QUEEN | NOTATION::COLOR::BLACK);
                    break;
                case 'p':
                    b.setField(pos, NOTATION::PIECES::PAWN | NOTATION::COLOR::BLACK);
                    break;
                case 'R':
                    b.setField(pos, NOTATION::PIECES::ROCK | NOTATION::COLOR::WHITE);
                    break;
                case 'N':
                    b.setField(pos, NOTATION::PIECES::KNIGHT | NOTATION::COLOR::WHITE);
                    break;
                case 'B':
                    b.setField(pos, NOTATION::PIECES::BISHOP | NOTATION::COLOR::WHITE);
                    break;
                case 'K':
                    b.setField(pos, NOTATION::PIECES::KING | NOTATION::COLOR::WHITE);
                    break;
                case 'Q':
                    b.setField(pos, NOTATION::PIECES::QUEEN | NOTATION::COLOR::WHITE);
                    break;
                case 'P':
                    b.setField(pos, NOTATION::PIECES::PAWN | NOTATION::COLOR::WHITE);
                break;
            }
            ++chessPos;
            ++strPos;
        }
    }

    ++strPos;

    if (fenStr[strPos] == 'w')
    {
        b.playerOnMove = NOTATION::COLOR::color::white;
    }	
    else
    {
        b.playerOnMove = NOTATION::COLOR::color::black;
    }
    ++++strPos;

    if (fenStr[strPos] != '-')
    {
        while (fenStr[strPos] != ' ')
        {
            switch (fenStr[strPos])
            {
                case 'k':
                    b.castlingRights |= NOTATION::CASTLING_RIGHTS::BLACK_SHORT_BIT;
                    break;
                case 'q':
                    b.castlingRights |= NOTATION::CASTLING_RIGHTS::BLACK_LONG_BIT;
                    break;
                case 'K':
                    b.castlingRights |= NOTATION::CASTLING_RIGHTS::WHITE_SHORT_BIT;
                    break;
                case 'Q':
                    b.castlingRights |= NOTATION::CASTLING_RIGHTS::BLACK_SHORT_BIT;
                    break;
            }
            ++strPos;
        }
    }
    be.hash_ = hash::hash(b);
    be.board = b;
    return be;
}

}  // namespace coordinates
}  // namespace notations
