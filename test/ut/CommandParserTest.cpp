#include <gtest/gtest.h>
#include <UciApplication/CommandParser.hpp>

#include <iostream>

namespace
{

template <typename TEvent>
struct VisitorMatcher
{
    VisitorMatcher(TEvent&& expectedEvent)
        : expectedEvent_(expectedEvent)
    {
    }

    bool operator()(TEvent&&)
    {
        return true;
    }    

    template <typename TOtherEvent>
    bool operator()(TOtherEvent&&)
    {
        return false;
    }
private:
    TEvent expectedEvent_;
};


struct VisitorPrint
{
    void operator()(GO go)
    {
        std::cout << "go"<< std::endl;
        std::cout << "go.timeForWhite: " << (go.timeForWhite ? std::to_string(*go.timeForWhite) : std::string("not set")) << std::endl;
        std::cout << "go.timeForBlack: " << (go.timeForBlack ? std::to_string(*go.timeForBlack) : std::string("not set")) << std::endl;
        std::cout << "go.movetime: " << (go.movetime ? std::to_string(*go.movetime) : std::string("not set")) << std::endl;
        std::cout << "go.movesToGo: " <<  go.movesToGo << std::endl;
    }

    template <typename TEvent>
    void operator()(TEvent&&)
    {
        std::cout << "Not defined event to print" << std::endl;
    }
};

template <typename TEvent>
auto makeVisitorMatcher(TEvent&& event)
{
    return VisitorMatcher<TEvent>(event);
}

template <typename TEventA, typename TEventB>
bool check(TEventA&& expectedCommand, TEventB&& givenCommand)
{
    return std::visit(makeVisitorMatcher(expectedCommand), givenCommand);
}

struct CommandParserShall
    : public ::testing::Test
{
    CommandParser parser;
};

}  // namespace

TEST_F(CommandParserShall, shallParseUciCommand)
{
    std::string commandStr = "uci";
    auto command = parser.parseCommand(commandStr);

    UCI expectedCommand{};
    ASSERT_TRUE(check(expectedCommand, command));

    IS_READY notExpectedCommand{};
    ASSERT_FALSE(check(notExpectedCommand, command));
}

TEST_F(CommandParserShall, shallParseIsReadyCommand)
{
    std::string commandStr = "isready";
    auto command = parser.parseCommand(commandStr);

    IS_READY expectedCommand{};
    ASSERT_TRUE(check(expectedCommand, command));
}

TEST_F(CommandParserShall, shallParseUciNewGameCommand)
{
    std::string commandStr = "ucinewgame";
    auto command = parser.parseCommand(commandStr);

    UCI_NEW_GAME expectedCommand{};
    ASSERT_TRUE(check(expectedCommand, command));
}

TEST_F(CommandParserShall, shallParseUciGoCommandWithAbsoluteTimeGiven)
{
    std::string commandStr = "go wtime 10000 btime 20000";
    auto command = parser.parseCommand(commandStr);

    GO expectedCommand{};
    expectedCommand.timeForWhite = 10000;
    expectedCommand.timeForBlack = 20000;

    ASSERT_TRUE(check(expectedCommand, command));
}
