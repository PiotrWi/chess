#pragma once

#include <memory>

#include "Events.hpp"

#include <BoardEngine.hpp>
#include <CachedEngines/FullCachedEngine.hpp>

class GameHandler
{
public:
    GameHandler();
    GameHandler(const char* evaluatorLocation, const char* evaluatorConfigurationLocation);
    void startProcessing();
    void onPositionProc(POSSITION&);
    void onGo(GO&);
    void interrupt();
    void emitBestMove(const Move&);
private:
    BoardEngine be;
    players::common::move_generators::FullCachedEngine cachedEngine;
};

extern std::unique_ptr<GameHandler> handler;

void handleUciNewGame(UCI_NEW_GAME&);
void handleSetOption(SET_OPTION& option);
