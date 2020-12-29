#pragma once

#include <publicIf/Board.hpp>
#include <publicIf/Notation.hpp>

namespace materialEvaluator
{
int evaluate(const Board &, NOTATION::COLOR::color);
}