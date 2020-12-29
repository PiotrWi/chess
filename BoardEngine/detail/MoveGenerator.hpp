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
	std::vector<Move> generate(const Board&
		, NOTATION::COLOR::color) const;
    std::vector<Move> generate(const Board&) const;

    std::vector<MoveDecorator> generateWithAdditionalInfo(const Board&) const;
};

}  // namespace MoveGenerator
