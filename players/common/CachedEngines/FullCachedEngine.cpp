#include "FullCachedEngine.hpp"

#include <common/evaluators/MaterialAndMoveCountEvaluator.hpp>

namespace players
{
namespace common
{
namespace move_generators
{

CacheFullEntity FullCachedEngine::get(const BoardEngine &be)
{
    auto hash = be.getHash();

    CacheFullEntity* elem;
    auto created = cache_.getOrCreate(hash, hash, elem);
    if (created)
    {
        elem->precalculatedMoves = be.generateMoves();
    }
    return *elem;
}

int FullCachedEngine::getEvaluationValue(const BoardEngine& be)
{
    auto hash = be.getHash();
    auto elem = cachedEvaluators_.get(hash, hash);
    if (!elem)
    {
        auto val = evaluateFunction(be);
        return *cachedEvaluators_.store(hash, hash, val);
    }
    return *elem;
}

void FullCachedEngine::setBestMove(const BoardEngine& be,
                 ExtendedMove& move,
                 unsigned char depth)
{
    auto hash = be.getHash();
    CacheFullEntity* elem = cache_.get(hash, hash);

    elem->previousBestMoves[depth].isSet = true;
    elem->previousBestMoves[depth].move = move;
}

void FullCachedEngine::setLowerBound(const BoardEngine &be, int value, unsigned char depth)
{
    auto hash = be.getHash();
    CacheFullEntity* elem = cache_.get(hash, hash);
    elem->previousEvaluations[depth].visitedBefore = true;
    elem->previousEvaluations[depth].lowerValue = value;
}

void FullCachedEngine::setUpperBound(const BoardEngine &be, int value, unsigned char depth)
{
    auto hash = be.getHash();
    CacheFullEntity* elem = cache_.get(hash, hash);
    elem->previousEvaluations[depth].visitedBefore = true;
    elem->previousEvaluations[depth].higherValue = value;
}

void FullCachedEngine::setLowerUpperBound(const BoardEngine &be, int valueMin, int valueMax, unsigned char depth)
{
    auto hash = be.getHash();
    CacheFullEntity* elem = cache_.get(hash, hash);
    elem->previousEvaluations[depth].visitedBefore = true;
    elem->previousEvaluations[depth].lowerValue = valueMin;
    elem->previousEvaluations[depth].higherValue = valueMax;
}

void FullCachedEngine::makeOlder()
{
    cache_.increaseAge();
    cachedEvaluators_.increaseAge();
}

void FullCachedEngine::clearOlderThan(unsigned char age)
{
    cache_.removeOlderThan(age);
    cachedEvaluators_.removeOlderThan(age);
}

}  // namespace players
}  // namespace common
}  // namespace move_generators
