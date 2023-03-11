#pragma once

#include "core/Board.hpp"

namespace utils
{

extern const char* InitialBoardString;

Board createBoard(const char* position,
        NOTATION::COLOR::color playerOnMove = NOTATION::COLOR::color::white);
void revokeCastlingRights(Board& board, unsigned char rightBit);
void setValidEnPassant(Board& board, const char* posStr);

uint64_t convertToUint64(const std::string& s);

}  // namespace utils
