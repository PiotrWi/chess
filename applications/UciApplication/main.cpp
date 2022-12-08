#include <string>
#include <thread>

#include "Events.hpp"
#include <utils/DebugWrapper.hpp>
#include <UciApplication/EventsPropagator.hpp>
#include <UciApplication/CommandParser.hpp>
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

struct CommandVisitor
{
    template <typename T>
    void operator()(T&& event)
    {
        eventPropagator.enqueue(event);
    }
    void operator()(QUIT&& q)
    {
        eventPropagator.enqueue(q);
        isDone_ = true;
    }
    bool isDone() const
    {
        return isDone_;
    }
private:
    bool isDone_ = false;
};

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
    CommandVisitor visitor;

    while ( not visitor.isDone() )
    {
        std::string command;
        std::getline(std::cin, command);
        debug.logCommand(command);   
        std::visit(visitor, parser.parseCommand(command));
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
