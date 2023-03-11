/* Note:
 * * May be inacurate in promotions situation - Not sure whether it change a lot.
 * * Do not consider checks - it would be to slow imo.
 */

#pragma once

#include <algorithm>
#include "core/Board.hpp"
#include "core/NotationConversions.hpp"
#include "MoveApplier/MoveApplier.hpp"
#include "core/bitboards/bitboardslookups.hpp"
#include "core/bitboards/BitBoardsUtils.h"

// It is bad
// These values shall be somehow global and configurable. It need to be chaned in further development.
constexpr int PawnValue = 100;
constexpr int KnightValue = 300;
constexpr int BishopValue = 300;
constexpr int RockValue = 500;
constexpr int QueenValue = 900;
constexpr int KingValue = 10000;
constexpr int PiecesValuesTable[] = {0, 0, PawnValue, PawnValue, RockValue, RockValue, KnightValue, KnightValue, BishopValue, BishopValue, QueenValue, QueenValue, KingValue, KingValue};

int see(ExtendedMove& move, Board board, NOTATION::COLOR::color playerColor);

