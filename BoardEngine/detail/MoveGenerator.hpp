#pragma once

#include <vector>

#include <publicIf/Board.hpp>

namespace MoveGenerator
{

struct MoveDecorator
{
    Move move_;
    bool isBeating;
    bool beatenValue;
};

class MoveGenerator
{
public:
	static std::vector<Move> generate(const Board&
		, NOTATION::COLOR::color);
    static  std::vector<Move> generate(const Board&);

    static std::vector<MoveDecorator> generateWithAdditionalInfo(const Board&);
};

}  // namespace MoveGenerator
