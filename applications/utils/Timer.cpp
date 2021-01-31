#include "Timer.hpp"
#include <UciApplication/Events.hpp>
#include <UciApplication/EventsPropagator.hpp>

#include <signal.h>
#include <cassert>
#include <iostream>

void (*UserHandler)();

void timerHandler2(union sigval)
{
    if (UserHandler != NULL)
    {
        UserHandler();
    }
}

int createTimer2(int microseconds, void(*callBack)())
{
    UserHandler = callBack;

    sigevent sig = {};
    timer_t timerId;
    sig.sigev_notify = SIGEV_THREAD;
    sig.sigev_notify_function = &timerHandler2;

    if (timer_create(CLOCK_REALTIME, &sig, &timerId))
    {
        std::cerr << "Timer crete failed" << std::endl;
        perror("errno");
        assert(0);
    }

    itimerspec input = {};
    input.it_value.tv_sec = microseconds / 1000000;
    input.it_value.tv_nsec = (microseconds % 1000000) * 1000;
    if (timer_settime(timerId, 0, &input, NULL))
    {
        std::cerr << "Timer start failed" << std::endl;
        perror("errno");
        assert(0);
    }
    
    return 0;
}