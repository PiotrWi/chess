#pragma once

#include <map>
#include <string>
#include <vector>

struct EVENT
{
public:
    virtual unsigned getId() = 0;
    virtual ~EVENT() = default;
};

struct QUIT : public EVENT
{
    static constexpr unsigned ID = 0;
    virtual unsigned getId() override
    {
        return ID;
    }
};

struct UCI : public EVENT
{
    static constexpr unsigned ID = 1;
    virtual unsigned getId() override
    {
        return ID;
    }
};

struct IS_READY : public EVENT
{
    static constexpr unsigned ID = 2;
    virtual unsigned getId() override
    {
        return ID;
    }
};

struct UCI_NEW_GAME : public EVENT
{
    UCI_NEW_GAME(const std::map<std::string, std::string>& optionsIn)
        : options(optionsIn) {}
    std::map<std::string, std::string> options;
    static constexpr unsigned ID = 3;
    virtual unsigned getId() override
    {
        return ID;
    }
};

struct POSSITION : public EVENT
{
public:
    POSSITION(bool isStartingIn, const std::string& fenStringIn, const std::vector<std::string>& movesIn)
        : isStarting(isStartingIn)
        , fenString(fenStringIn)
        , moves(movesIn) {}
    bool isStarting = false;
    std::string fenString;
    std::vector<std::string> moves;

    static constexpr unsigned ID = 4;
    virtual unsigned getId() override
    {
        return ID;
    }
};

struct GO : public EVENT
{
public:
    static constexpr unsigned ID = 5;
    virtual unsigned getId() override
    {
        return ID;
    }

    int timeForWhite = 0;
    int timeForBlack = 0;
    int movesToGo = 0; /*Not sure what it is*/
};

struct BEST_MOVE : public EVENT
{
public:
    static constexpr unsigned ID = 6;
    virtual unsigned getId() override
    {
        return ID;
    }

    BEST_MOVE(std::string bestMoveIn)
        : bestMove(bestMoveIn) {}
    std::string bestMove;
};

constexpr unsigned ID_COUNT = 7;
