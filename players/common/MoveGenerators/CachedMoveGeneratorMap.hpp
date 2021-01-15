#pragma once

#include <vector>

#include <BoardEngine.hpp>
#include <common/containers/HashMap.hpp>

namespace players
{
namespace common
{
namespace move_generators
{

class CachedMoveGeneratorMap
{
public:
    const std::vector<ExtendedMove>& generate(const BoardEngine& be);
    const std::vector<ExtendedMove>& generate(const BoardEngine& be, NOTATION::COLOR::color);

    void makeOlder();
    void clearOlderThan(unsigned char age);
    void setKillerMove(const BoardEngine& be, unsigned int index);
private:
    containers::HashMap<std::vector<ExtendedMove>, Board, 24u> cache_;
};

}  // namespace players
}  // namespace common
}  // namespace move_generators
