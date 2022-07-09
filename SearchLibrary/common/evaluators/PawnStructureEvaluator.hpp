#pragma once

#include <publicIf/Board.hpp>
#include <publicIf/Notation.hpp>

namespace pawnStructureEvaluator
{

int evaluate(const Board &, NOTATION::COLOR::color playerOnMove);

}