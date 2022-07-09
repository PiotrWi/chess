#pragma once

#include <memory>
#include <IPLayer.hpp>
#include <BoardEngine.hpp>

class ConsoleApplication
{
public:
    ConsoleApplication();

    void setWhitePlayer(std::unique_ptr<IPlayer>&&);
    void setBlackPlayer(std::unique_ptr<IPlayer>&&);
    void start();

private:
    bool validateAndReject(const Move &move, IPlayer &player);

    std::unique_ptr<IPlayer> whitePlayer_;
    std::unique_ptr<IPlayer> blackPlayer_;
    BoardEngine boardEngine_;
};
