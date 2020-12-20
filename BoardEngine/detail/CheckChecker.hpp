#pragma once

#include <publicIf/Notation.hpp>

class Board;

namespace CheckChecker
{

bool isAttackedOn(const Board& board,
		NOTATION::COLOR::color playerColor,
		unsigned char field);
bool isCheckOn(const Board& board,
		const NOTATION::COLOR::color kingColor);

}