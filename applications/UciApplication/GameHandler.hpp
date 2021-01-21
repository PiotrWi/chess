#pragma once

#include <memory>

#include "Events.hpp"

#include <BoardEngine.hpp>
#include <common/CachedEngines/CachedEngineWithMap.hpp>

class GameHandler
{
public:
    GameHandler();
    void startProcessing();
    void onPositionProc(POSSITION&);
    void onGo(GO&);
    void emitBestMove(const Move& );
private:
    BoardEngine be;
    players::common::move_generators::CachedEngineWithMap cachedEngine;
};

extern std::unique_ptr<GameHandler> handler;

void handleUciNewGame(UCI_NEW_GAME&);
