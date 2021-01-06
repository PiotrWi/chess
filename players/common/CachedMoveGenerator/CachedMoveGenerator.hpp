#pragma once

#include <vector>

#include <BoardEngine.hpp>
#include <common/containers/HashTable.hpp>

class CachedMoveGenerator
{
public:
    const std::vector<Move> generate(const BoardEngine& be);
private:
    containers::HashTable<std::vector<Move>, Board, 24u> cache_;
};
