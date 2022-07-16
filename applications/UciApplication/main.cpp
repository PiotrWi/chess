#include <string>
#include <thread>
#include <sstream>

#include "Events.hpp"
#include <utils/DebugWrapper.hpp>
#include <UciApplication/EventsPropagator.hpp>
#include "GameHandler.hpp"

void handleUci(UCI&)
{
    debug << "id name PiotorChess_bitBoard v0.1";
    debug << "id author https://github.com/PiotrWi";
    debug << "option name customEvaluator type string default <empty>";
    debug << "option name evaluatorConfig type string default <empty>";
    debug << "uciok";
}

void handleIsReady(IS_READY&)
{
    debug << "readyok";
}

void handleBestMove(BEST_MOVE& bestMove)
{
    debug << "bestmove " + bestMove.bestMove;
}

void readCommands()
{
    std::map <std::string, std::string> options;
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
            eventPropagator.enqueue(UCI_NEW_GAME{options});
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
                ss >> word;
                if (word == "moves")
                {
                    while (!ss.eof())
                    {
                        ss >> word;
                        moves.push_back(word);
                    }
                }
                auto event = POSSITION{true, {}, moves};
                eventPropagator.enqueue(event);
            }
            else if (word == "fen")
            {
                std::string fenString = "";
                ss >> word;
                fenString += word;
                while (ss >> word, !ss.eof() && word != "moves")
                {
                    fenString += " ";
                    fenString += word;
                }
                while (!ss.eof())
                {
                    ss >> word;
                    moves.push_back(word);
                }
                auto event = POSSITION{false, fenString, moves};
                eventPropagator.enqueue(event);
            }
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
                    go.timeForWhite = std::stoi(word);
                }

                if (word.find("btime") != std::string::npos)
                {
                    ss >> word;
                    go.timeForBlack = std::stoi(word);
                }

                if (word.find("movetime") != std::string::npos)
                {
                    ss >> word;
                    go.movetime = std::stoi(word);
                }
            }
            eventPropagator.enqueue(go);
        }
        if (command == "quit")
        {
            QUIT quit;
            eventPropagator.enqueue(quit);
            return;
        }
        if (command.find("setoption") != std::string::npos)
        {
            std::string word;
            std::stringstream ss(command);
            ss >> word; // consume "option" string
            ss >> word; // consume "name" string
            ss >> word;
            auto key = word;
            ss >> word; // consume "value" string
            ss >> word;
            options [key] = word;
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

    debug.logInDebug("Exit application\n");
    return 0;
}
