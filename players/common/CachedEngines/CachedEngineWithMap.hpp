#pragma once

#include <vector>

#include <BoardEngine.hpp>
#include <common/containers/HashMap.hpp>

namespace players
{
namespace common
{
namespace move_generators
{

struct CacheEntity
{
    /*!
     * \brief Killer moves remember the best moves from the past executions. THe heuristic is that if move was
     * previously good, it shall be good also now.
     *
     * If no move stored as killer, there can be returned move from different depth.
     *
     * killerMove changes the order of moves returned by CachingEngine:
     * There are 3 possibilities:
     *  - No killer at all. returnedKiller is equal to 0.
     *    - Precondition: KillerMoves {depth = d, killerInNode = -1, returnedKiller = 0, returnedFromDifferentNode = f}
     *    - State before moves are return: {depth = d, killerInNode = 0, returnedKiller = 0, returnedFromDifferentNode = f}
     *    - MovesOrder is as usual:  { #0, #1, #2, #3, .... #{n-1} }
     *    - Then no special handling is needed. returnedKiller == 0, setupKiller just stores an index inside.
     *  - There is killer a killer for given node
     *    - Precondition: KillerMoves {depth = d, killerInNode = X, returnedKiller = Y, returnedFromDifferentNode = f}
     *    - State before moves are return: {depth = d, killerInNode = X, returnedKiller = X, returnedFromDifferentNode = f}
     *    - Move order is: {#K, #{K+1}, .... #{n-1}, #0, #1, #2 ... #{K-1}}
     *    - Then no special handling is needed. returnedKiller == 0, setupKiller just stores an index inside.
     *  - Utilizing killer from lower node.
     *    - Precondition: KillerMoves {depth = d, killerInNode = -1, returnedKiller = 0, returnedFromDifferentNode = f}
     *    - State before moves are return: {depth = d, killerInNode = 0, returnedKiller = X, returnedFromDifferentNode = t}
     *    - Move order is: {#K, #0, .... #{K-1}, #{K-1}, #{N-1}}
     */
    struct KillerMoves
    {
        signed char killerInNode = -1;
        signed char returnedKiller = -1;
        bool returnedFromDifferentNode = false;
    };

    /**
     *
     */
    struct PreviousEvaluations
    {
        bool betaCutOff;
        bool alfaCutOff;
        int evaluationValue;
    };
    std::vector<ExtendedMove> precalculatedMoves;
    KillerMoves killers[10]{{-1, -1, false},
                            {-1, -1, false},
                            {-1, -1, false},
                            {-1, -1, false},
                            {-1, -1, false},
                            {-1, -1, false},
                            {-1, -1, false},
                            {-1, -1, false},
                            {-1, -1, false},
                            {-1, -1, false}};
};

constexpr CacheEntity::KillerMoves InitialKiller = {0, 0, false};


class CachedEngineWithMap
{
public:
    const std::vector<ExtendedMove> generate(const BoardEngine& be,
                                              unsigned char depth);
    const std::vector<ExtendedMove> generate(const BoardEngine& be,
                                              NOTATION::COLOR::color,
                                              unsigned char depth);

    int getEvaluationValue(const BoardEngine& be);

    void setKillerMove(const BoardEngine& be,
                       unsigned int index,
                       unsigned char depth);

    void makeOlder();
    void clearOlderThan(unsigned char age);
    void clear();
private:
    containers::HashMap<CacheEntity, Board, 22u> cache_;
    containers::HashMap<int, Board, 22u> cachedEvaluators_;
};

}  // namespace players
}  // namespace common
}  // namespace move_generators
