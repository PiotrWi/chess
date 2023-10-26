#include "CachedPlayer.hpp"
#include <cstring>
#include <notations/coordinationnotation.hpp>
#include <searchingAlgorithms/FullSearchingImplementation.hpp>

CachedPlayer::CachedPlayer() {}
CachedPlayer::CachedPlayer(const char* customEvaluatorLocation, const char* customEvaluatorConfigurationLocation)
    : cmg_(customEvaluatorLocation, customEvaluatorConfigurationLocation)
{
}

void CachedPlayer::init(Color)
{
}

const char* CachedPlayer::act(const char *string)
{
    if (string != nullptr)
    {
        be.applyMove(notations::coordinates::createExtendedMove(string, be.board.playerOnMove, be.board));
    }
    auto move = full_search::evaluateIterative(be, cmg_, 10);
    be.applyMove(move);

    cmg_.clear();

    strcpy(lastMove_, notations::coordinates::createMoveStr(move.operator Move()).data());
    return lastMove_;
}

void CachedPlayer::rejectLast()
{

}
