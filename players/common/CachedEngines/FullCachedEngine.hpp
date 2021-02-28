#pragma once

#include <vector>

#include <BoardEngine.hpp>
#include <common/containers/HashMap.hpp>

static constexpr unsigned MAX_DEPTH = 12;

namespace players
{
namespace common
{
namespace move_generators
{

struct CacheFullEntity
{
    struct PreviousBestMove
    {
        bool isSet = false;
        ExtendedMove move;
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

    // std::vector<ExtendedMove> precalculatedMoves = {};

    PreviousBestMove previousBestMoves[MAX_DEPTH] = {};
    PreviousEvaluations previousEvaluations[MAX_DEPTH] = {};
};

class FullCachedEngine
{
public:
    CacheFullEntity& get(const BoardEngine& be);

    int getEvaluationValue(const BoardEngine& be, unsigned int validMovesCount);
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
};

}  // namespace players
}  // namespace common
}  // namespace move_generators
