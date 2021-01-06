#include "SimpleComputerPlayer.hpp"
#include <cstring>
#include <notations/LongAlgebraicNotation.hpp>
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
        be.applyMove(createMove(string, be.board.playerOnMove));
    }
    auto move = alfaBeta::evaluate(be, cachedMoveGenerator_, 6);
    be.applyMove(move);
    strcpy(lastMove_, createMoveStr(move).data());
    return lastMove_;
}

void SimpleComputerPlayer::rejectLast()
{

}
