#include <string>
#include <thread>
#include <sstream>

#include "Events.hpp"
#include <utils/DebugWrapper.hpp>
#include <UciApplication/EventsPropagator.hpp>
#include "GameHandler.hpp"

void handleUci(UCI&)
{
    debug << "id name PiotorChess";
    debug << "id author https://github.com/PiotrWi";
    debug << "uciok";
}

void handleIsReady(IS_READY&)
{
    debug << "readyok";
}

void handleBestMove(BEST_MOVE& bestMove)
{
    debug << "bestMove " + bestMove.bestMove;
}

void readCommands()
{
    eventPropagator.registerToEvent(handleUci);
    eventPropagator.registerToEvent(handleIsReady);
    eventPropagator.registerToEvent(handleUciNewGame);
    eventPropagator.registerToEvent(handleBestMove);

    while (true)
    {
        std::string command;
        std::getline(std::cin, command);
        debug.logCommand(command);

        if (command == "uci")
        {
            eventPropagator.enqueue(UCI{});
        }
        if (command == "isready")
        {
            eventPropagator.enqueue(IS_READY{});
        }
        if (command == "ucinewgame")
        {
            eventPropagator.enqueue(UCI_NEW_GAME{});
        }
        if (command.find("position") != std::string::npos)
        {
            std::string word;
            std::stringstream ss(command);
            ss >> word;
            ss >> word;
            std::vector<std::string> moves;
            if (word == "startpos")
            {
                while (!ss.eof())
                {
                    ss >> word;
                    moves.push_back(word);
                }
            }
            auto event = POSSITION{false, moves};
            eventPropagator.enqueue(event);
        }

        if (command.find("go") != std::string::npos)
        {
            std::string word;
            std::stringstream ss(command);
            ss >> word;

            GO go;
            while (not ss.eof())
            {
                ss >> word;
                if (word.find("wtime") != std::string::npos)
                {
                    ss >> word;
                    std::cout << word << std::endl;
                    go.timeForWhite = std::stoi(word);
                }

                if (word.find("btime") != std::string::npos)
                {
                    ss >> word;
                    std::cout << word << std::endl;
                    go.timeForBlack = std::stoi(word);
                }
            }
            eventPropagator.enqueue(go);
        }
        if (command == "quit")
        {
            return;
        }
    }
}

int main(int, char**)
{
    std::thread t([](){
        eventPropagator.start();
    });

    readCommands();

    t.join();
    return 0;
}
