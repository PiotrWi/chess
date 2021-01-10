#pragma once

#include <vector>

#include <BoardEngine.hpp>
#include <common/containers/HashMap.hpp>

class CachedMoveGeneratorMap
{
public:
    const std::vector<ExtendedMove>* generate(const BoardEngine& be);
private:
    containers::HashMap<std::vector<ExtendedMove>, Board, 24u> cache_;
};
