#pragma once

#include <publicIf/Notation.hpp>

struct Board;

namespace CheckChecker
{

unsigned char findKing(const Board& board, const NOTATION::COLOR::color c) noexcept;

bool isAttackedBySliders(const Board& board,
                                 NOTATION::COLOR::color playerColor,
                                 unsigned char field) noexcept;

bool isAttackedOn(const Board& board,
		NOTATION::COLOR::color playerColor,
		unsigned char field) noexcept;

bool isCheckOn(const Board& board,
		const NOTATION::COLOR::color kingColor) noexcept;

struct AttackOut
{
    /*
        Number of pieces attacking the king.
        In practive:
            0: King not attacked
            1: King attacked by single piece.
                There is a possibility to remove a check by: beat an attacker or block it.
                Naturally, king can be moved to another piece.
            2: King is attacked by 2 pieces. Then, only king can be moved.
    */
    unsigned char attackersNum = 0;
    /*
        Valid in situation when attackersNum==1. In other situation, no sense to read it.
        Moving any not pined piece (other than king obviouslu) to this field removed a check.
    */
    uint64_t possibleBlockersMask = 0ull;
};

AttackOut isCheckedBeforeMoveExtendeded(const Board& board,
        NOTATION::COLOR::color playerColor,
        unsigned char fieldPosition) noexcept;
}
