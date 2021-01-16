#include "CachedEngineWithMap.hpp"

#include <utility>
#include <publicIf/NotationConversions.hpp>

#include <common/evaluators/MaterialAndMoveCountEvaluator.hpp>

namespace players
{
namespace common
{
namespace move_generators
{

const std::vector<ExtendedMove> CachedEngineWithMap::generate(const BoardEngine &be, unsigned char depth)
{
    auto hash = be.getHash();
    CacheEntity* elem;
    auto created = cache_.getOrCreate(hash, be.board, elem);
    if (created)
    {
        elem->precalculatedMoves = be.generateMoves();
        elem->killers[depth] = InitialKiller;
        return elem->precalculatedMoves;
    }

    if (elem->precalculatedMoves.empty())
    {
        return {};
    }

    if (elem->killers[depth].killerInNode > 0)
    {
        elem->killers[depth].returnedKiller = elem->killers[depth].killerInNode;
        elem->killers[depth].returnedFromDifferentNode = false;
        std::vector<ExtendedMove> moves;
        moves.reserve(elem->precalculatedMoves.size());
        moves.insert(moves.end(),
                     elem->precalculatedMoves.begin() + elem->killers[depth].killerInNode,
                     elem->precalculatedMoves.end());
        moves.insert(moves.end(),
                     elem->precalculatedMoves.begin(),
                     elem->precalculatedMoves.begin() + elem->killers[depth].killerInNode);
        return moves;
    }
    else
    {
        for (auto candidateDepth = depth - 1; candidateDepth > 0; --candidateDepth)
        {
            if (elem->killers[candidateDepth].killerInNode != -1)
            {
                auto killer = elem->killers[candidateDepth].killerInNode;
                elem->killers[depth].returnedKiller = killer;
                elem->killers[depth].returnedFromDifferentNode = true;

                std::vector<ExtendedMove> moves;
                moves.reserve(elem->precalculatedMoves.size());
                moves.push_back(elem->precalculatedMoves[killer]);

                moves.insert(moves.end(),
                             elem->precalculatedMoves.begin(),
                             elem->precalculatedMoves.begin() + killer);
                moves.insert(moves.end(),
                             elem->precalculatedMoves.begin() + killer + 1,
                             elem->precalculatedMoves.end());
                return moves;
            }
        }
    }

    elem->killers[depth] = InitialKiller;
    return elem->precalculatedMoves;
}

const std::vector<ExtendedMove> CachedEngineWithMap::generate(const BoardEngine& be,
                                                               NOTATION::COLOR::color c,
                                                               unsigned char depth)
{
    auto hash = be.getHash(c);

    auto* elem = cache_.get(hash, be.board);
    if (!elem)
    {
        CacheEntity entity;
        entity.precalculatedMoves = be.generateMovesFor(c);
        entity.killers[depth] = InitialKiller;
        return cache_.store(hash, be.board, std::move(entity))->precalculatedMoves;
    }

    if (elem->precalculatedMoves.empty())
    {
        return {};
    }


    if (elem->killers[depth].killerInNode > 0)
    {
        elem->killers[depth].returnedKiller = elem->killers[depth].killerInNode;
        elem->killers[depth].returnedFromDifferentNode = false;
        std::vector<ExtendedMove> moves;
        moves.reserve(elem->precalculatedMoves.size());
        moves.insert(moves.end(),
                     elem->precalculatedMoves.begin() + elem->killers[depth].killerInNode,
                     elem->precalculatedMoves.end());
        moves.insert(moves.end(),
                     elem->precalculatedMoves.begin(),
                     elem->precalculatedMoves.begin() + elem->killers[depth].killerInNode);
        return moves;
    }
    else
    {
        for (auto candidateDepth = depth - 1; candidateDepth > 0; --candidateDepth)
        {
            if (elem->killers[candidateDepth].killerInNode != -1)
            {
                auto killer = elem->killers[candidateDepth].killerInNode;
                elem->killers[depth].returnedKiller = killer;
                elem->killers[depth].returnedFromDifferentNode = true;

                std::vector<ExtendedMove> moves;
                moves.reserve(elem->precalculatedMoves.size());
                moves.push_back(elem->precalculatedMoves[killer]);

                moves.insert(moves.end(),
                             elem->precalculatedMoves.begin(),
                             elem->precalculatedMoves.begin() + killer);
                moves.insert(moves.end(),
                             elem->precalculatedMoves.begin() + killer + 1,
                             elem->precalculatedMoves.end());
                return moves;
            }
        }
    }

    elem->killers[depth] = InitialKiller;
    return elem->precalculatedMoves;
}

int CachedEngineWithMap::getEvaluationValue(const BoardEngine& be)
{
    auto hash = be.getHash();
    auto elem = cachedEvaluators_.get(hash, be.board);
    if (!elem)
    {
        auto val = evaluateFunction(be);
        return *cachedEvaluators_.store(hash, be.board, val);
    }
    return *elem;
}

void CachedEngineWithMap::makeOlder()
{
    cache_.increaseAge();
    cachedEvaluators_.increaseAge();
}

void CachedEngineWithMap::clearOlderThan(unsigned char age)
{
    cache_.removeOlderThan(age);
    cachedEvaluators_.removeOlderThan(age);
}

void clear()
{
    clear();
}

void CachedEngineWithMap::setKillerMove(const BoardEngine& be, unsigned int index, unsigned char depth)
{
    auto hash = be.getHash();
    auto* node = cache_.get(hash, be.board);

    if (node->killers[depth].returnedFromDifferentNode == false)
    {
        node->killers[depth].killerInNode = (node->killers[depth].returnedKiller + index) % node->precalculatedMoves.size();
    }
    else
    {
        // {#K, #0, .... #{K-1}, #{K-1}, #{N-1}}
        if (index == 0)
        {
            node->killers[depth].killerInNode = node->killers[depth].returnedKiller;
        }
        else if (index <= (unsigned char)node->killers[depth].killerInNode)
        {
            node->killers[depth].killerInNode = index -1;
        }
        else
        {
            node->killers[depth].killerInNode = index;
        }
    }
}


}  // namespace players
}  // namespace common
}  // namespace move_generators
