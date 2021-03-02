#pragma once

#include "fen.h"

#include <ctype.h>
#include <string>

#include <BoardEngine.hpp>

namespace notations
{
namespace coordinates
{

    // rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
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
                ++strPos;
                chessPos += fenStr[strPos] - '0';
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
            }
            ++chessPos;
            ++strPos;
        }
    }



    while()
    return be;
}

}  // namespace coordinates
}  // namespace notations
