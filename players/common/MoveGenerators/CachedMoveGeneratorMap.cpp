#include "CachedMoveGeneratorMap.hpp"

#include <utility>

namespace players
{
namespace common
{
namespace move_generators
{

const std::vector<ExtendedMove>& CachedMoveGeneratorMap::generate(const BoardEngine &be)
{
    auto hash = be.getHash();

    auto elem = cache_.get(hash, be.board);
    if (!elem)
    {
        auto moves = be.generateMoves();
        return *cache_.store(hash, be.board, moves);

    }
    return *elem;
}

const std::vector<ExtendedMove>& CachedMoveGeneratorMap::generate(const BoardEngine& be, NOTATION::COLOR::color c)
{
    auto hash = be.getHash(c);

    auto elem = cache_.get(hash, be.board);
    if (!elem)
    {
        auto moves = be.generateMovesFor(c);
        return *cache_.store(hash, be.board, moves);
    }
    return *elem;
}

void CachedMoveGeneratorMap::makeOlder()
{
    cache_.increaseAge();
}

void CachedMoveGeneratorMap::clearOlderThan(unsigned char age)
{
    cache_.removeOlderThan(age);

}

void CachedMoveGeneratorMap::setKillerMove(const BoardEngine& be, unsigned int index)
{
    auto hash = be.getHash();
    auto node = cache_.get(hash, be.board);

    std::swap((*node)[0], (*node)[index]);
}


}  // namespace players
}  // namespace common
}  // namespace move_generators
