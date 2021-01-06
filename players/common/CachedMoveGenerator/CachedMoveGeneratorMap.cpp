#include "CachedMoveGeneratorMap.hpp"

#include <utility>

const std::vector<Move>* CachedMoveGeneratorMap::generate(const BoardEngine &be)
{
    auto hash = be.getHash();

    auto elem = cache_.get(hash, be.board);
    if (!elem)
    {
        auto moves = be.generateMoves();
        return cache_.store(hash, be.board, std::move(moves));
    }
    return elem;
}