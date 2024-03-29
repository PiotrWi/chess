#include "ConsoleApplication.hpp"

#include <cassert>
#include <iostream>
#include "core/BoardIO.hpp"

#include <notations/coordinationnotation.hpp>

void ConsoleApplication::setWhitePlayer(std::unique_ptr<IPlayer>&& player)
{
    whitePlayer_ = std::move(player);
}

void ConsoleApplication::setBlackPlayer(std::unique_ptr<IPlayer>&& player)
{
    blackPlayer_ = std::move(player);
}

bool ConsoleApplication::validateAndReject(const Move &move, IPlayer &player)
{
    auto validationResult = boardEngine_.validateMove(move);
    if (!validationResult)
    {
        player.rejectLast();
    }
    return validationResult;
}

static const char* toString(Result result)
{
    if (result == Result::blackWon)
    {
        return "black won";
    }
    if (result == Result::ongoing)
    {
        return "ongoing";
    }
    if (result == Result::draw)
    {
        return "draw";
    }
    if (result == Result::whiteWon)
    {
        return "white won";
    }
    return "";
}

void ConsoleApplication::start()
{
    assert(whitePlayer_.operator bool());
    assert(blackPlayer_.operator bool());

    const char* lastMoveStr = nullptr;
    ExtendedMove lastMove;

    while (true)
    {
        for (auto& colorAndPlayer: {
            std::pair<NOTATION::COLOR::color, IPlayer&>{NOTATION::COLOR::color::white, *whitePlayer_},
            std::pair<NOTATION::COLOR::color, IPlayer&>{NOTATION::COLOR::color::black, *blackPlayer_}})
        {
            std::cout << boardEngine_.board << std::endl;
            auto& player = colorAndPlayer.second;
            auto& color = colorAndPlayer.first;

            const char* lastMoveTmp = "";

            do
            {
                lastMoveTmp = player.act(lastMoveStr);
                std::cout << "player move is: " << lastMoveTmp << std::endl;
                lastMove = notations::coordinates::createExtendedMove(lastMoveTmp, color, boardEngine_.board);
            } while(!validateAndReject(lastMove.operator Move(), *whitePlayer_));
            lastMoveStr = lastMoveTmp;
            boardEngine_.applyMove(lastMove);
            auto result = boardEngine_.getResult();
            if (result != Result::ongoing)
            {
                std::cout << "game ended with result " << toString(result) << std::endl;
                return;
            }
        }
    }
}


