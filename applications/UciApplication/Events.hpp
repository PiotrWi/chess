#pragma once

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

    BEST_MOVE(const std::string& bestMoveIn)
        : bestMove(bestMoveIn) {}
    std::string bestMove;
};

struct SET_OPTION : public EVENT
{
public:
    static constexpr unsigned ID = 7;
    virtual unsigned getId() override
    {
        return ID;
    }

    SET_OPTION(const std::string& keyIn, const std::string& valueIn)
        : key(keyIn)
        , value(valueIn)
        {}
    std::string key;
    std::string value;
};

struct IS_QUIET : public EVENT
{
public:
    static constexpr unsigned ID = 8;
    virtual unsigned getId() override
    {
        return ID;
    }
    IS_QUIET(std::string fenStringIn)
        : fenString(fenStringIn)
        {}
    std::string fenString;
};

struct QUIET_RESP : public EVENT
{
public:
    static constexpr unsigned ID = 9;
    virtual unsigned getId() override
    {
        return ID;
    }
    QUIET_RESP(bool isQuietIn)
        : isQuiet(isQuietIn)
    {}
    bool isQuiet;
};

constexpr unsigned ID_COUNT = 10;
