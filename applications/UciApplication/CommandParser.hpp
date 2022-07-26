#pragma once

#include <string>
#include <variant>
#include "Events.hpp"

using TCommandLineOptionsVariant = std::variant<UCI, IS_READY, UCI_NEW_GAME, POSSITION, GO, SET_OPTION, QUIT>;
class CommandParser
{
public:
    TCommandLineOptionsVariant parseCommand(const std::string& command) const;
};
