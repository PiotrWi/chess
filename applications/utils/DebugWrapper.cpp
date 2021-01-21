#include "DebugWrapper.hpp"

#include <unistd.h>
#include <cassert>
#include <filesystem>

#include <iostream>

DebugWrapper::DebugWrapper()
{
    std::filesystem::create_directory("/tmp/UciApplication/");
    std::filesystem::create_directory("/tmp/UciApplication/" + std::to_string(getpid()));
    std::string debugLocation = "/tmp/UciApplication/" + std::to_string(getpid()) + "/debug.log";
    f.open(debugLocation, std::ios::out);

    if(not f.is_open())
    {
        std::cerr << debugLocation << std::endl;
        perror("Debug file not open.");
        assert(f.is_open());
    }
}

DebugWrapper& DebugWrapper::operator << (const std::string& s)
{
    std::cout << s << std::endl;
    f << "out: " << s << std::endl;

    return *this;
}

void DebugWrapper::logCommand(const std::string& s)
{
    f << "in: " << s << std::endl;
}

void DebugWrapper::logInDebug(const std::string& s)
{
    f << "debug: " << s << std::endl;
}

DebugWrapper debug;
