#pragma once

#include <vector>
#include <common/Constants.hpp>
#include <BoardEngine.hpp>
#include <common/containers/HashMap.hpp>

namespace players
{
namespace common
{
namespace move_generators
{
using TEvaluatePositionHandler = int (*)(BoardEngine& be, unsigned int validMovesCount);
using TInitHandler = void (*)(const char*);

struct CacheFullEntity
{
    struct PreviousBestMove
    {
        bool isSet = false;
        ExtendedMove move;
        PreviousBestMove()
            : isSet(false)
            , move() {}
    };

    struct PreviousEvaluations
    {
        PreviousEvaluations()
            : visitedBefore(false)
            , lowerValue(-10000001)
            , higherValue(10000001) {}
        bool visitedBefore = false;
        int lowerValue = -10000001;
        int higherValue = 10000001;
    };

    PreviousBestMove previousBestMoves[MAX_DEPTH] = {};
    PreviousEvaluations previousEvaluations[MAX_DEPTH] = {};
};

class FullCachedEngine
{
public:
    FullCachedEngine();
    FullCachedEngine(const char* evaluatorLibLocation, const char* evaluatorConfig);
    ~FullCachedEngine();

    FullCachedEngine(const FullCachedEngine&) = delete;
    FullCachedEngine operator=(const FullCachedEngine&) = delete;
    FullCachedEngine(FullCachedEngine&&) = delete;
    FullCachedEngine operator=(FullCachedEngine&&) = delete;

    CacheFullEntity* get(const BoardEngine& be);

    int getEvaluationValue(BoardEngine& be, unsigned int validMovesCount);
    void setBestMove(const BoardEngine& be,
                       ExtendedMove& move,
                       unsigned char depth);

    void setLowerBound(const BoardEngine &be, int value, unsigned char depth);
    void setUpperBound(const BoardEngine &be, int value, unsigned char depth);
    void setLowerUpperBound(const BoardEngine &be, int valueMin, int valueMax, unsigned char depth);

    void makeOlder();
    void clearOlderThan(unsigned char age);

    void clear();
private:
    containers::HashMap<CacheFullEntity, uint64_t , 22u> cache_;
    containers::HashMap<int, uint64_t, 22u> cachedEvaluators_;

    TEvaluatePositionHandler evaluatePositionHandler;
    TInitHandler initHandler;
    void *libHandle = nullptr;
};

}  // namespace players
}  // namespace common
}  // namespace move_generators
