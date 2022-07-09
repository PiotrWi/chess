#pragma once

#include <IPLayer.hpp>

class HumanPlayer : public IPlayer
{
public:
    void init(Color c) override;
    const char* act(const char*) override;
    void rejectLast() override;
private:
    Color playerColor_;
    char lastMove_[20];
};
