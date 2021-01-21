#pragma once
#include <fstream>

class DebugWrapper
{
public:
    DebugWrapper();
    DebugWrapper& operator << (const std::string&);
    void logCommand(const std::string&);
    void logInDebug(const std::string&);
    std::ofstream f;
};

extern DebugWrapper debug;
