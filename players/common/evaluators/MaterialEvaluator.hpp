#pragma once

#include <publicIf/Board.hpp>
#include <publicIf/Notation.hpp>

namespace materialEvaluator
{
signed char evaluate(const Board &, NOTATION::COLOR::color);
}