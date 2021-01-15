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
    const std::vector<ExtendedMove> generate(const BoardEngine& be) const;
    const std::vector<ExtendedMove> generate(const BoardEngine& be, NOTATION::COLOR::color) const;
    void setKillerMove(const BoardEngine&, unsigned int) {}

    unsigned getMoveCount(const BoardEngine& be) const;
    unsigned getMoveCount(const BoardEngine& be, NOTATION::COLOR::color) const;
};

}  // namespace move_generators
}  // namespace common
}  // namespace players
