#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>

#include "Events.hpp"

class EventPropagator
{
public:
    EventPropagator();

    template<class TEvent>
    void enqueue(TEvent event)
    {
        std::lock_guard<std::mutex> lk(taskQueueMutex_);

        taskQueue_.push([&, event](){
            auto id = TEvent::ID;
            eventsHandler_[id]((TEvent&)event);
        });
        cv.notify_one();
    }


    template<class TEvent>
    void registerToEvent(void (*fcn)(TEvent&))
    {
        std::lock_guard<std::mutex> lk(taskQueueMutex_);

        auto eventId = TEvent::ID;

        auto fcnWrapper = [fcn](EVENT& ev){
            auto& concreteEv = dynamic_cast<TEvent&>(ev);
            fcn(concreteEv);
        };

        eventsHandler_[eventId] = fcnWrapper;
    }

    void start();
private:
    std::vector<std::function<void(EVENT&)>> eventsHandler_;
    std::mutex taskQueueMutex_;
    std::queue<std::function<void()>> taskQueue_;
    std::condition_variable cv;
};

extern EventPropagator eventPropagator;
