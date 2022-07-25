#include <string>
#include <thread>
#include <sstream>

#include "Events.hpp"
#include <utils/DebugWrapper.hpp>
#include <UciApplication/EventsPropagator.hpp>
#include "GameHandler.hpp"
#include <variant>

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

using TCommandLineOptionsVariant = std::variant<UCI, IS_READY, UCI_NEW_GAME, POSSITION, GO, SET_OPTION, QUIT>;
class CommandParser
{
public:
    TCommandLineOptionsVariant parseCommand(const std::string& command) const;
};

TCommandLineOptionsVariant CommandParser::parseCommand(const std::string& command) const
{
    if (command == "uci")
    {
        return UCI{};
    }
    if (command == "isready")
    {
        return IS_READY{};
    }
    if (command == "ucinewgame")
    {
        return UCI_NEW_GAME{};
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
            return POSSITION{true, {}, moves};
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
            return POSSITION{false, fenString, moves};
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
        return go;
    }
    if (command == "quit")
    {
        return QUIT{};
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
        return SET_OPTION(key, word);
    }
    return {};
}

void registerLocalCommands()
{
    eventPropagator.registerToEvent(handleUci);
    eventPropagator.registerToEvent(handleIsReady);
    eventPropagator.registerToEvent(handleBestMove);
}

void registerGameHandlerCommands()
{
    eventPropagator.registerToEvent(handleUciNewGame);
    eventPropagator.registerToEvent(handleSetOption);
}

void readCommands()
{
    CommandParser parser;
    while (true)
    {
        std::string command;
        std::getline(std::cin, command);
        debug.logCommand(command);

        std::visit([&](auto&& arg) {eventPropagator.enqueue(arg);}, parser.parseCommand(command));
    }
}

int main(int, char**)
{
    std::thread t([](){
        eventPropagator.start();
    });

    registerLocalCommands();
    registerGameHandlerCommands();
    readCommands();

    t.join();

    debug.logInDebug("Exit application\n");
    return 0;
}
