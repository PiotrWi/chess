#include <gtest/gtest.h>

#include <ConsoleApplication.hpp>
#include <ComputerPlayer/SimpleComputerPlayer.hpp>

TEST(SimpleGame, Simulate)
{
    ConsoleApplication app;
    app.setWhitePlayer(std::make_unique<SimpleComputerPlayer>());
    app.setBlackPlayer(std::make_unique<SimpleComputerPlayer>());

    app.start();
}