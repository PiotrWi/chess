#pragma once

#include <vector>

#include <publicIf/Board.hpp>

namespace MoveGenerator
{

class MoveGenerator
{
public:
	static std::vector<ExtendedMove> generate(Board&
		, NOTATION::COLOR::color);
    static std::vector<ExtendedMove> generate(Board&);
    static unsigned getMoveCount(Board&
            , NOTATION::COLOR::color);
    static unsigned getMoveCount(Board&);
};

}  // namespace MoveGenerator
