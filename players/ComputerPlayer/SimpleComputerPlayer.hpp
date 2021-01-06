#pragma once

#include <common/IPLayer.hpp>
#include <BoardEngine.hpp>
#include <common/CachedMoveGenerator/CachedMoveGenerator.hpp>
#include <common/CachedMoveGenerator/CachedMoveGeneratorMap.hpp>

class SimpleComputerPlayer
        : public IPlayer
{
public:
    const char *act(const char *string) override;
    void rejectLast() override;
    void init(Color c) override;
private:
    Color playerColor_;
    char lastMove_[20];
    BoardEngine be;

    CachedMoveGenerator cachedMoveGenerator_;
};
