#include <BoardEngine.hpp>

#include <cassert>
#include <cstring>
#include <iostream>
#include <HumanPlayer/HumanPlayer.hpp>
#include <HumanPlayer/SimpleComputerPlayer.hpp>
#include "ConsoleApplication.hpp"

std::unique_ptr<IPlayer> createPlayer(const char* playerType)
{
    if (strcmp(playerType, "humanPlayer") == 0)
    {
        return std::make_unique<HumanPlayer>();
    }
    if (strcmp(playerType, "simpleComputerPlayer") == 0)
    {
        return std::make_unique<SimpleComputerPlayer>();
    }
    assert(false);
    return {};
}

int main(int argc, char** argv)
{
    if (argc == 1)
    {
        std::cout << "Incorrect usage. You need to specify the players." << std::endl;
        std::cout << "Syntax is: " << std::endl;
        std::cout << "./chess --whitePlayer playerType --blackPlayer playerType" << std::endl;
        std::cout << "where player types are:" << std::endl;
        std::cout << "  humanPlayer" << std::endl;
        std::cout << "  simpleComputerPlayer" << std::endl;
        return 1;
    }

    auto app = ConsoleApplication();
    for (auto i = 0; i < argc; ++i)
    {
        if (strcmp(argv[i], "--whitePlayer") == 0)
        {
            auto player = createPlayer(argv[++i]);
            player->init(Color::white);
            app.setWhitePlayer(std::move(player));
        }
        if (strcmp(argv[i], "--blackPlayer") == 0)
        {
            auto player = createPlayer(argv[++i]);
            player->init(Color::black);
            app.setBlackPlayer(std::move(player));
        }
    }

    app.start();

	return 0;
}
