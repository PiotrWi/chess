#include <string>
#include <thread>
#include <sstream>
#include "Events.hpp"
#include <utils/DebugWrapper.hpp>
#include <UciApplication/EventsPropagator.hpp>
#include "GameHandler.hpp"
#include "OptionHandler.hpp"
#include "IsQuietHandler.hpp"

void handleUci(UCI&)
{
    debug << "id name PiotorChess_bitBoard v0.2";
    debug << "id author https://github.com/PiotrWi";
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

void handleQuietResp(QUIET_RESP& quietResp)
{
    debug << std::string("quiet_response ") + (quietResp.isQuiet ? "true" : "false");
}

void readCommands()
{
    eventPropagator.registerToEvent(handleUci);
    eventPropagator.registerToEvent(handleIsReady);
    eventPropagator.registerToEvent(handleUciNewGame);
    eventPropagator.registerToEvent(handleBestMove);
    eventPropagator.registerToEvent(handleQuietResp);

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
            }
            eventPropagator.enqueue(go);
        }

        if (command == "quit")
        {
            QUIT quit;
            eventPropagator.enqueue(quit);
            return;
        }

        if (command.find("option") != std::string::npos)
        {
            std::string word;
            std::stringstream ss(command);
            ss >> word;

            std::string key;
            std::string value;

            ss >> key;
            if (not ss.eof())
            {
                ss >> value;
            }

            eventPropagator.enqueue(SET_OPTION(key, value));
        }

        // custom UCI extensions
        if (command.find("quiet") != std::string::npos)
        {
            std::string word;
            std::string fenString = "";
            std::stringstream ss(command);
            ss >> word;

            while (ss >> word)
            {
                fenString += word;
                if (ss.eof())
                    break;
                fenString += " ";
            }

            eventPropagator.enqueue(IS_QUIET(fenString));
        }
    }
}

int main(int, char**)
{
    optionHandler.registerHandler();
    isQuietHandler.registerHandler();

    std::thread t([](){
        eventPropagator.start();
    });

    readCommands();

    t.join();

    debug.logInDebug("Exit application\n");
    return 0;
}
