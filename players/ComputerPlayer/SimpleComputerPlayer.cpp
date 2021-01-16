#include "SimpleComputerPlayer.hpp"
#include <cstring>
#include <notations/coordinationnotation.hpp>
#include <common/searchingAlgorithms/AlfaBeta.hpp>
#include <common/CachedEngines/MoveGenerator.hpp>

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

    auto mg = players::common::move_generators::MoveGenerator();
    auto move = alfaBeta::evaluate(be, mg, 6);
    be.applyMove(move);
    strcpy(lastMove_, notations::coordinates::createMoveStr(move).data());
    return lastMove_;
}

void SimpleComputerPlayer::rejectLast()
{

}
