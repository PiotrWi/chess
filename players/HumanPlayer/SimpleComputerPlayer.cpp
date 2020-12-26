#include "SimpleComputerPlayer.hpp"
#include <cstring>
#include <notations/LongAlgebraicNotation.hpp>
#include <common/searchingAlgorithms/MinMax.hpp>

void SimpleComputerPlayer::init(Color c)
{
    playerColor_ = c;
}

const char *SimpleComputerPlayer::act(const char *string)
{
    if (string != nullptr)
    {
        be.applyMove(createMove(string, be.board.playerOnMove));
    }
    auto move = minMax::evaluate(be, 4);
    be.applyMove(move);
    strcpy(lastMove_, createMoveStr(move).data());
    return lastMove_;
}

void SimpleComputerPlayer::rejectLast()
{

}
