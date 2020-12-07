#pragma once

#include <vector>

#include <Board.hpp>

namespace MoveGenerator
{

class MoveGenerator
{
	std::vector<Move> generate(const Board&) const;
};

}  // namespace MoveGenerator
