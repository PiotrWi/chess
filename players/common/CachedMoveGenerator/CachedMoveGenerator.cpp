#include "CachedMoveGenerator.hpp"

const std::vector<Move> CachedMoveGenerator::generate(const BoardEngine &be)
{
    auto hash = be.getHash();

    auto elem = cache_.get(hash, be.board);
    if (!elem)
    {
        auto moves = be.generateMoves();
        cache_.store(hash, be.board, moves);
        return moves;
    }
    return elem->second;
}