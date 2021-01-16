#pragma once

#include <common/IPLayer.hpp>
#include <BoardEngine.hpp>
#include <common/CachedEngines/CachedEngineWithMap.hpp>

class CachedPlayer
    : public IPlayer
{
public:
    const char *act(const char *string) override;
    void rejectLast() override;
    void init(Color c) override;
private:
    char lastMove_[20];
    BoardEngine be;
    players::common::move_generators::CachedEngineWithMap cmg_;
};
