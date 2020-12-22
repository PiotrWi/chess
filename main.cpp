#include <BoardEngine.hpp>

#include <cstring>
#include <iostream>
#include <HumanPlayer/HumanPlayer.hpp>
#include "ConsoleApplication.hpp"

int main(int argc, char** argv)
{
    if (argc == 1)
    {
        std::cout << "Incorrect usage. You need to specify the players." << std::endl;
        std::cout << "Currently the only suported is" << std::endl;
        std::cout << "./chess --whitePlayer humanPlayer --blackPlayer humanPlayer" << std::endl;
        return 1;
    }

    auto app = ConsoleApplication();
    for (auto i = 0; i < argc; ++i)
    {
        if (strcmp(argv[i], "--whitePlayer"))
        {
            auto player = std::make_unique<HumanPlayer>();
            player->init(Color::white);
            app.setWhitePlayer(std::move(player));
            continue;
        }
        if (strcmp(argv[i], "--blackPlayer"))
        {
            auto player = std::make_unique<HumanPlayer>();
            player->init(Color::black);
            app.setBlackPlayer(std::move(player));
            continue;
        }
    }

    app.start();

	return 0;
}
