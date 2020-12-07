#pragma once

#include <vector>

#include <Board.hpp>

namespace MoveGenerator
{

class MoveGenerator
{
public:
	std::vector<Move> generate(const Board&) const;
};

}  // namespace MoveGenerator
