#pragma once

#include <publicIf/Board.hpp>

namespace utils
{

extern const char* InitialBoardString;

Board createBoard(const char* position,
        NOTATION::COLOR::color playerOnMove = NOTATION::COLOR::color::white);
void revokeCastlingRights(Board& board, unsigned char rightBit);
void setValidEnPassant(Board& board, const char* posStr);

}  // namespace utils
