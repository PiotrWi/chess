#pragma once

#include <vector>
#include "Constants.hpp"
#include "BoardEngine.hpp"
#include "non_std/containers/HashMap.hpp"
#include "non_std/containers/FixedSizeHashTableOpenHashingWithAge.hpp"

struct PassTrhoughtHash
{
    uint64_t operator()(uint64_t in) {return in;}
};

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

    void setBestMove(const ExtendedMove& move, unsigned char depth);
    void setLowerBound(int value, unsigned char depth);
    void setUpperBound(int value, unsigned char depth);
    void setLowerUpperBound(int valueMin, int valueMax, unsigned char depth);
};

using TCacheType = non_std::containers::HashMap<CacheFullEntity, uint64_t , 22u>;
// using TCacheType = non_std::containers::FixedSizeHashTableOpenHashingWithAge<uint64_t, CacheFullEntity, PassTrhoughtHash>;

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

    TCacheType::pointer get(const BoardEngine& be);

    int getEvaluationValue(BoardEngine& be, unsigned int validMovesCount);

    void clear();
private:
    TCacheType cache_;
    non_std::containers::HashMap<int, uint64_t, 22u> cachedEvaluators_;

    TEvaluatePositionHandler evaluatePositionHandler;
    TInitHandler initHandler;
    void *libHandle = nullptr;
};

}  // namespace players
}  // namespace common
}  // namespace move_generators
