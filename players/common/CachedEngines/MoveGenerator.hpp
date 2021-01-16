#pragma once

#include <BoardEngine.hpp>

namespace players
{
namespace common
{
namespace move_generators
{

class MoveGenerator
{
public:
    const std::vector<ExtendedMove> generate(const BoardEngine& be, unsigned char) const;
    const std::vector<ExtendedMove> generate(const BoardEngine& be, NOTATION::COLOR::color, unsigned char) const;
    void setKillerMove(const BoardEngine&, unsigned int, unsigned char) {}
    int getEvaluationValue(const BoardEngine& be);
};

}  // namespace move_generators
}  // namespace common
}  // namespace players
