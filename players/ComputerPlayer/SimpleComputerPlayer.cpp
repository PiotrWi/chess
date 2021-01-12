#include "SimpleComputerPlayer.hpp"
#include <cstring>
#include <notations/coordinationnotation.hpp>
#include <common/searchingAlgorithms/MinMax.hpp>
#include <common/searchingAlgorithms/AlfaBeta.hpp>
#include <common/searchingAlgorithms/AlfaBetaWithTransitctionTable.hpp>
#include <common/searchingAlgorithms/AlfaBetaTransictionTableMap.hpp>

void SimpleComputerPlayer::init(Color c)
{
    playerColor_ = c;
}

const char *SimpleComputerPlayer::act(const char *string)
{
    if (string != nullptr)
    {
        be.applyMove(notations::coordinates::createMove(string, be.board.playerOnMove));
    }
//    cachedMoveGenerator_ = {};
    auto move = alfaBeta::evaluate(be, cachedMoveGenerator_, 8);
    be.applyMove(move);
    strcpy(lastMove_, notations::coordinates::createMoveStr(move).data());
    return lastMove_;
}

void SimpleComputerPlayer::rejectLast()
{

}
