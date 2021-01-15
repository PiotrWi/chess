#include "CachedMoveGenerator.hpp"

namespace players
{
namespace common
{
namespace move_generators
{

const std::vector<ExtendedMove> CachedMoveGenerator::generate(const BoardEngine &be)
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

const std::vector<ExtendedMove> CachedMoveGenerator::generate(const BoardEngine& be, NOTATION::COLOR::color c)
{
    auto hash = be.getHash(c);

    auto elem = cache_.get(hash, be.board);
    if (!elem)
    {
        auto moves = be.generateMovesFor(c);

        cache_.store(hash, be.board, moves);
        return moves;
    }
    return elem->second;
}


}  // namespace players
}  // namespace common
}  // namespace move_generators
