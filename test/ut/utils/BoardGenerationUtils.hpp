#pragma once

#include <Board.hpp>

namespace utils
{

extern const char* InitialBoardString;

Board createBoard(const char* position,
        NOTATION::COLOR::color playerOnMove = NOTATION::COLOR::color::white);
void setMovedBit(Board&, const char*);
void setLastMove(Board&, const Move&);

}  // namespace utils
