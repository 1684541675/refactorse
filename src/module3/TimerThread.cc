#include "TimerThread.h"
#include <bits/stdc++.h>
#include <iostream>

using namespace std;

TimerThread::TimerThread(TimerCallBack &&_cb, int init, int periodic)
:_timer(move(_cb), init, periodic)
,_thread(bind(&Timer::start, &_timer))
{

}

void TimerThread::start()
{
    _thread.create();
}

void TimerThread::stop()
{
    _timer.stop();
    _thread.join();
}

