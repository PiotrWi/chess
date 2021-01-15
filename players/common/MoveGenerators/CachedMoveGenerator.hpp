#pragma once

#include <vector>

#include <BoardEngine.hpp>
#include <common/containers/HashTable.hpp>

namespace players
{
namespace common
{
namespace move_generators
{

class CachedMoveGenerator
{
public:
    const std::vector<ExtendedMove> generate(const BoardEngine& be);
    const std::vector<ExtendedMove> generate(const BoardEngine& be, NOTATION::COLOR::color);
    void setKillerMove(const BoardEngine&, unsigned int) {}
private:
    containers::HashTable<std::vector<ExtendedMove>, Board, 24u> cache_;
};

}  // namespace players
}  // namespace common
}  // namespace move_generators
