#include "TimerThread.h"
#include <utility>
using std::move;

namespace searchengine
{

TimerThread::TimerThread(TimerCallBack &&cb, int init, int periodic)
:_timer(move(cb), init, periodic)
,_thread([this](){ _timer.start(); })
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

}
