#pragma once

enum class Color
{
    white,
    black,
};

class IPlayer
{
public:
    virtual void init(Color  c) = 0;
    virtual const char* act(const char*) = 0;
    virtual void rejectLast() = 0;
};
