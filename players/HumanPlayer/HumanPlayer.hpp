#pragma once

#include <common/IPLayer.hpp>

class HumanPlayer : public IPlayer
{
public:
    void init(Color c) override;
    const char* act(const char*) override;
    void rejectLast() override;
private:
    Color playerColor_;
};
