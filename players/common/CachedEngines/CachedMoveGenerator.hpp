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
    const std::vector<ExtendedMove> generate(const BoardEngine& be, unsigned char);
    const std::vector<ExtendedMove> generate(const BoardEngine& be, NOTATION::COLOR::color, unsigned char);
    void setKillerMove(const BoardEngine&, unsigned int, unsigned char) {}
    int getEvaluationValue(const BoardEngine& be);
private:
    containers::HashTable<std::vector<ExtendedMove>, Board, 18u> cache_;
    containers::HashTable<int, Board, 18u> cachedEvaluators_;
};

}  // namespace players
}  // namespace common
}  // namespace move_generators
