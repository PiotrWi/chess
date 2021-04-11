#pragma once

#include <BoardEngine.hpp>
#include <common/CachedEngines/FullCachedEngine.hpp>

class IsQuietHandler
{
public:
    void registerHandler();
public:
    void handle(const std::string& fenString);
private:
    BoardEngine be;
    players::common::move_generators::FullCachedEngine cachedEngine;
};

extern IsQuietHandler isQuietHandler;
