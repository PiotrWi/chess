#include "FullCachedEngine.hpp"

#include "evaluatorIf.hpp"

#include <dlfcn.h>

namespace players
{
namespace common
{
namespace move_generators
{

void CacheFullEntity::setBestMove(const ExtendedMove& move, unsigned char depth)
{
    previousBestMoves[depth].isSet = true;
    previousBestMoves[depth].move = move;
}

void CacheFullEntity::setLowerBound(short value, unsigned char depth)
{
    previousEvaluations[depth].visitedBefore = true;
    previousEvaluations[depth].lowerValue = value;
}

void CacheFullEntity::setUpperBound(short value, unsigned char depth)
{
    previousEvaluations[depth].visitedBefore = true;
    previousEvaluations[depth].higherValue = value;
}

void CacheFullEntity::setLowerUpperBound(short valueMin, short valueMax, unsigned char depth)
{
    previousEvaluations[depth].visitedBefore = true;
    previousEvaluations[depth].lowerValue = valueMin;
    previousEvaluations[depth].higherValue = valueMax;
}

FullCachedEngine::FullCachedEngine()
{
    evaluatePositionHandler = evaluatePosition;
    initHandler = init;
    initHandler("");
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

TCacheType::pointer FullCachedEngine::get(const BoardEngine &be)
{
    auto hash = be.getHash();

    return cache_[hash];
}

int FullCachedEngine::getEvaluationValue(BoardEngine& be, unsigned int validMovesCount)
{
    auto hash = be.getHash();
    auto elem = cachedEvaluators_.get(hash);
    if (!elem)
    {
        auto val = evaluatePositionHandler(be, validMovesCount);
        return *cachedEvaluators_.store(hash, val);
    }
    return *elem;
}

void FullCachedEngine::clear()
{
    cache_ = {};

    cachedEvaluators_ = {};
}

}  // namespace players
}  // namespace common
}  // namespace move_generators
