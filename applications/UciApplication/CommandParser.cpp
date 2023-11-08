#include "CommandParser.hpp"
#include <sstream>

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
    // setoption option name customEvaluator value /home/pioter/proj/chess/bin/libmlEvaluator.so
    // setoption option name evaluatorConfig value /home/pioter/proj/chess/ml-poc/chess500r100r1rDO.tf
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