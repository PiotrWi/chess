#include "CachedPlayer.hpp"
#include <cstring>
#include <notations/coordinationnotation.hpp>
#include <common/searchingAlgorithms/AlfaBetaPvs.hpp>

void CachedPlayer::init(Color)
{
}

const char* CachedPlayer::act(const char *string)
{
    if (string != nullptr)
    {
        be.applyMove(notations::coordinates::createMove(string, be.board.playerOnMove));
    }
    auto move = alfaBetaPvs::evaluateIterative(be, cmg_, 8);
    be.applyMove(move);

    cmg_ = {};

    strcpy(lastMove_, notations::coordinates::createMoveStr(move).data());
    return lastMove_;
}

void CachedPlayer::rejectLast()
{

}
