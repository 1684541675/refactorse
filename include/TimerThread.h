#pragma once
#include "Thread.h"
#include "Timer.h"

/*************************************************************
 *
 *  timer 线程
 *
 *************************************************************/
namespace searchengine
{

class TimerThread
{
    using TimerCallBack = function<void()>;

public:
    TimerThread(TimerCallBack &&, int, int);

    void start();
    void stop();

private:
    Timer _timer;
    Thread _thread;
};

}
