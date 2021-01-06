#pragma once

#include <BoardEngine.hpp>
#include <common/CachedMoveGenerator/CachedMoveGeneratorMap.hpp>

namespace alfaBeta
{

Move evaluate(BoardEngine, CachedMoveGeneratorMap&, unsigned char depth);

}  // namespace alfaBeta
