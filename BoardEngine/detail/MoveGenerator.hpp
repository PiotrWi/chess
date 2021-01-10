#pragma once

#include <vector>

#include <publicIf/Board.hpp>

namespace MoveGenerator
{

class MoveGenerator
{
public:
	static std::vector<ExtendedMove> generate(const Board&
		, NOTATION::COLOR::color);
    static  std::vector<ExtendedMove> generate(const Board&);
};

}  // namespace MoveGenerator
