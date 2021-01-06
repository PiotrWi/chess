#pragma once

#include <BoardEngine.hpp>
#include <common/CachedMoveGenerator/CachedMoveGenerator.hpp>
namespace alfaBeta
{

Move evaluate(BoardEngine, CachedMoveGenerator&, unsigned char depth);

}  // namespace alfaBeta
