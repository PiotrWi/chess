#pragma once

#include <optional>
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
    UCI_NEW_GAME() {}

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

    std::optional<int> timeForWhite = std::nullopt;
    std::optional<int> timeForBlack = std::nullopt;
    std::optional<int> movetime = std::nullopt;
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

struct SET_OPTION : public EVENT
{
public:
    static constexpr unsigned ID = 7;
    virtual unsigned getId() override
    {
        return ID;
    }

    SET_OPTION(std::string keyIn, std::string valueIn)
        : key(keyIn)
        , value(valueIn) {}
    std::string key;
    std::string value;
};

constexpr unsigned ID_COUNT = 8;
