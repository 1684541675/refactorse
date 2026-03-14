#pragma once


#include <iostream>
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <poll.h>
using namespace std;

class Timer
{
    using TimerCallBack = function<void()>;

public:
    Timer(TimerCallBack &&, int, int);

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

