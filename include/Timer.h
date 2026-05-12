#pragma once
#include <functional>
using std::function;

namespace searchengine
{

class Timer
{
    using TimerCallBack = function<void()>;

public:
    Timer(TimerCallBack &&, int, int);
    ~Timer();
    
    void start();
    void stop();

private:
    int createTimerFd();
    void setTimerFd(int, int);
    void handleRead();

private:
    TimerCallBack _timerCb;
    int _timerFd;
    int _initTime;
    int _periodicTime;
    bool _isRunning;
};

} // namespace searchengine