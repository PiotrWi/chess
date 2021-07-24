#include "GameHandler.hpp"
#include <UciApplication/EventsPropagator.hpp>
#include <notations/uci.hpp>
#include <notations/fen.h>
#include <common/searchingAlgorithms/FullSearchingImplementation.hpp>
#include <utils/Timer.hpp>
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

void onStopProccessing()
{
    handler->interrupt();
}

void GameHandler::interrupt()
{
    full_search::interrupt();
    debug.logInDebug("Interrupted");
}

void GameHandler::onGo(GO& goEvent)
{
    cachedEngine = {};

    unsigned remainingTime;
    if (be.board.playerOnMove == NOTATION::COLOR::color::white)
    {
        remainingTime = goEvent.timeForWhite;
    }
    else
    {
        remainingTime = goEvent.timeForBlack;
    }
    debug.logInDebug("starting for us:" + std::to_string(70 * remainingTime)); // 8% remaining time
    createTimer2(70*remainingTime, onStopProccessing);

    auto move = full_search::evaluateIterative(be, cachedEngine, 20);
    emitBestMove(move);
}

void GameHandler::emitBestMove(const Move& move)
{
    auto event = BEST_MOVE{notations::uci::createMoveStr(convertMoveToExtended(be.board, move)).data()};
    eventPropagator.enqueue(event);
}

GameHandler::GameHandler()
{
}

void GameHandler::startProcessing()
{
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
            auto move = notations::uci::createExtendedMove(moveStr, be.board.playerOnMove, be.board);
            be.applyMove(move);
        }
    }
    else
    {
        be = notations::fen::initByFen(event.fenString);
        for (const auto& moveStr : event.moves)
        {
            auto move = notations::uci::createExtendedMove(moveStr, be.board.playerOnMove, be.board);
            be.applyMove(move);
        }
    }
}

void handleUciNewGame(UCI_NEW_GAME&)
{
    handler = std::make_unique<GameHandler>();
    handler->startProcessing();
}
