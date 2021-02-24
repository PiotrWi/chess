#include "CachedPlayer.hpp"
#include <cstring>
#include <notations/coordinationnotation.hpp>
#include <common/searchingAlgorithms/FullSearchingImplementation.hpp>

void CachedPlayer::init(Color)
{
}

const char* CachedPlayer::act(const char *string)
{
    if (string != nullptr)
    {
        be.applyMove(notations::coordinates::createExtendedMove(string, be.board.playerOnMove, be.board));
    }
    auto move = full_search::evaluateIterative(be, cmg_, 8);
    be.applyMove(move);

    cmg_ = {};

    strcpy(lastMove_, notations::coordinates::createMoveStr(move).data());
    return lastMove_;
}

void CachedPlayer::rejectLast()
{

}
