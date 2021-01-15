#include "CachedPlayer.hpp"
#include <cstring>
#include <notations/coordinationnotation.hpp>
#include <common/searchingAlgorithms/AlfaBeta.hpp>

void CachedPlayer::init(Color)
{
}

const char* CachedPlayer::act(const char *string)
{
    if (string != nullptr)
    {
        be.applyMove(notations::coordinates::createMove(string, be.board.playerOnMove));
    }

    auto move = alfaBeta::evaluate(be, cmg_, 8);
    be.applyMove(move);

    cmg_.makeOlder();
    cmg_.clearOlderThan(3u);

    strcpy(lastMove_, notations::coordinates::createMoveStr(move).data());
    return lastMove_;
}

void CachedPlayer::rejectLast()
{

}
