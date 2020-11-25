#pragma once

#include <Notation.hpp>

class Board;

namespace CheckChecker
{

bool isCheckOn(const Board& board, const NOTATION::COLOR::color c);

}
