#include "UciApplication/EventsPropagator.hpp"
#include "Events.hpp"

namespace
{
bool quit;

void setQuit(QUIT&)
{
    quit = true;
}
}  // namespace

EventPropagator::EventPropagator()
    : eventsHandler_(ID_COUNT)
{
}

void EventPropagator::start()
{
    quit = false;
    this->registerToEvent(setQuit);

    while (!quit)
    {
        std::vector<std::function<void()>> toCall;
        {
            std::unique_lock <std::mutex> lock(taskQueueMutex_);
            cv.wait(lock,[&](){
                return not taskQueue_.empty();
            });
            while (!taskQueue_.empty())
            {
                toCall.emplace_back(taskQueue_.front());
                taskQueue_.pop();
            }
        }
        for (auto& task: toCall) task();
    }
}

EventPropagator eventPropagator;
