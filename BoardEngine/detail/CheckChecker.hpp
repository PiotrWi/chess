#pragma once

#include <publicIf/Notation.hpp>

struct Board;

namespace CheckChecker
{
unsigned char findKing(const Board& board, const NOTATION::COLOR::color c);

bool isAttackedOn(const Board& board,
		NOTATION::COLOR::color playerColor,
		unsigned char field);
bool isCheckOn(const Board& board,
		const NOTATION::COLOR::color kingColor);

}
