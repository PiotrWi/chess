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
    alfaBetaPvs::evaluate(be, cmg_, 2);
    alfaBetaPvs::evaluate(be, cmg_, 4);
    alfaBetaPvs::evaluate(be, cmg_, 6);
    auto move = alfaBetaPvs::evaluate(be, cmg_, 8);
    be.applyMove(move);

    cmg_ = {};
//    cmg_.makeOlder();
//    cmg_.clearOlderThan(2u);

    strcpy(lastMove_, notations::coordinates::createMoveStr(move).data());
    return lastMove_;
}

void CachedPlayer::rejectLast()
{

}
