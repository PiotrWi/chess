#pragma once

#include <publicIf/Notation.hpp>

struct Board;

namespace CheckChecker
{
unsigned char findKing(const Board& board, const NOTATION::COLOR::color c) noexcept;

bool isAttackedOnRapid(const Board& board, NOTATION::COLOR::color c, unsigned char fieldPosition);

bool isAttackedOn(const Board& board,
		NOTATION::COLOR::color playerColor,
		unsigned char field) noexcept;
bool isCheckOn(const Board& board,
		const NOTATION::COLOR::color kingColor) noexcept;

}
