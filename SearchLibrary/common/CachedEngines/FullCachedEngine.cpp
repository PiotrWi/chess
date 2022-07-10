#include "FullCachedEngine.hpp"

#include <evaluatorIf.hpp>

#include <dlfcn.h>

namespace players
{
namespace common
{
namespace move_generators
{

FullCachedEngine::FullCachedEngine()
{
    evaluatePositionHandler = evaluatePosition;
    initHandler = init;
}

FullCachedEngine::FullCachedEngine(const char* evaluatorLibLocation, const char* evaluatorConfig)
{
    libHandle = dlopen(evaluatorLibLocation, RTLD_LAZY);

    if (!libHandle)
    {
        throw std::runtime_error("provider library not exist");
        return;
    }
    evaluatePositionHandler = (TEvaluatePositionHandler)dlsym(libHandle, "evaluatePosition");
    initHandler = (TInitHandler)dlsym(libHandle, "init");
    initHandler(evaluatorConfig);
}

FullCachedEngine::~FullCachedEngine()
{
    if (libHandle)
    {
        dlclose(libHandle);
    }
}

CacheFullEntity* FullCachedEngine::get(const BoardEngine &be)
{
    auto hash = be.getHash();

    CacheFullEntity* elem;
    cache_.getOrCreate(hash, hash, elem);
    return elem;
}

int FullCachedEngine::getEvaluationValue(BoardEngine& be, unsigned int validMovesCount)
{
    auto hash = be.getHash();
    auto elem = cachedEvaluators_.get(hash, hash);
    if (!elem)
    {
        auto val = evaluatePositionHandler(be, validMovesCount);
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

void FullCachedEngine::clear()
{
    cache_ = {};
    cachedEvaluators_ = {};
}

}  // namespace players
}  // namespace common
}  // namespace move_generators
