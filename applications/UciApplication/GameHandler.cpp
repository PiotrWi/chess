#include "GameHandler.hpp"
#include <UciApplication/EventsPropagator.hpp>
#include <notations/LongAlgebraicNotation.hpp>
#include <common/searchingAlgorithms/AlfaBetaPvs.hpp>

#include <utils/DebugWrapper.hpp>

std::unique_ptr<GameHandler> handler;

void onPositionProcHandler(POSSITION& event)
{
    handler->onPositionProc(event);
}

void onGoHandler(GO& event)
{
    handler->onGo(event);
}

void GameHandler::onGo(GO&)
{
    cachedEngine = {};
    auto move = alfaBetaPvs::evaluateIterative(be, cachedEngine, 2);
    emitBestMove(move);
}

void GameHandler::emitBestMove(const Move& move)
{
    auto event = BEST_MOVE{notations::long_algebraic::createMoveStr(convertMoveToExtended(be.board, move)).data()};
    eventPropagator.enqueue(event);
}

GameHandler::GameHandler()
{
}

void GameHandler::startProccessing()
{
    debug.logInDebug("startProccessing");
    eventPropagator.registerToEvent(onPositionProcHandler);
    eventPropagator.registerToEvent(onGoHandler);
}

void GameHandler::onPositionProc(POSSITION& event)
{
    if (event.isStarting == true)
    {
        be = {};
        for (const auto& moveStr : event.moves)
        {
            auto move = notations::long_algebraic::createExtendedMove(moveStr, be.board.playerOnMove, be.board);
            be.applyMove(move);
        }
    }
}

void handleUciNewGame(UCI_NEW_GAME&)
{
    handler = std::make_unique<GameHandler>();
    handler->startProccessing();
}
