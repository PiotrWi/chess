#include "GameHandler.hpp"
#include <UciApplication/EventsPropagator.hpp>
#include <notations/uci.hpp>
#include <notations/fen.h>
#include <common/searchingAlgorithms/FullSearchingImplementation.hpp>
#include <utils/Timer.hpp>
#include <utils/DebugWrapper.hpp>
#include <thread>

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

int getTimeForMoveFromEvent(const GO& event, NOTATION::COLOR::color playerOnMove)
{
    if (event.movetime)
    {
        return *event.movetime;
    }
    if (playerOnMove == NOTATION::COLOR::color::white && event.timeForWhite)
    {
        return *event.timeForWhite;
    }
    if (playerOnMove == NOTATION::COLOR::color::black && event.timeForBlack)
    {
        return *event.timeForBlack;
    }
    return 0;
}

void GameHandler::onGo(GO& goEvent)
{
    cachedEngine.clear();

    unsigned remainingTime = getTimeForMoveFromEvent(goEvent, be.board.playerOnMove);

    debug.logInDebug("starting for us:" + std::to_string(70 * remainingTime)); // 8% remaining time
    createTimer2(70*remainingTime, onStopProccessing);

    auto move = full_search::evaluateIterative(be, cachedEngine, 20);
    emitBestMove(move.operator Move());
}

void GameHandler::emitBestMove(const Move& move)
{
    auto event = BEST_MOVE{notations::uci::createMoveStr(convertMoveToExtended(be.board, move)).data()};
    eventPropagator.enqueue(event);
}

GameHandler::GameHandler()
{
}

GameHandler::GameHandler(const char* evaluatorLocation, const char* evaluatorConfigurationLocation)
    : cachedEngine(evaluatorLocation, evaluatorConfigurationLocation)
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

void handleUciNewGame(UCI_NEW_GAME& newGameEvent)
{
    if (newGameEvent.options.find("customEvaluator") != newGameEvent.options.end())
    {
        const char* evaluatorLocation = newGameEvent.options.at("customEvaluator").c_str();
        const char* evaluatorConfigurationLocation = nullptr;
        if (newGameEvent.options.find("evaluatorConfig")!= newGameEvent.options.end())
        {
            evaluatorConfigurationLocation = newGameEvent.options.at("evaluatorConfig").c_str();
        }
        handler = std::make_unique<GameHandler>(evaluatorLocation, evaluatorConfigurationLocation);
        handler->startProcessing();
        return;
    }
    handler = std::make_unique<GameHandler>();
    handler->startProcessing();
}
