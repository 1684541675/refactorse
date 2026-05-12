#include "Timer.h"

#include <unistd.h>
#include <sys/timerfd.h>
#include <poll.h>

#include <utility>    // std::move
#include <cerrno>     // errno, EINTR
#include <cstdio>     // perror, printf
#include <cstdint>    // uint64_t

using std::move;

#define CHECK_RET(ret, msg) \
    do { \
        if ((ret) == -1) { \
            perror(msg); \
            exit(EXIT_FAILURE); \
        } \
    } while (0)

namespace searchengine
{

Timer::Timer(TimerCallBack &&cb, int init, int periodic)
:_timerCb(move(cb))
,_timerFd(-1)//   _timerFd(createTimerFd()), // 若在这儿 create timerFd，它只属于主线程！
,_initTime(init)
,_periodicTime(periodic)
,_isRunning(false)
{

}

Timer::~Timer()
{
    if (_timerFd >= 0)
    {
        close(_timerFd);
    }
}

/**
 *  1. 子线程调用
 *  2. 创建 _timerFd
 *  3. 设置定时器时间
 *  4. 开始监听
 */
void Timer::start()
{
    _timerFd = createTimerFd();           // 2
    setTimerFd(_initTime, _periodicTime); // 3

    struct pollfd pfd;
    pfd.events = POLLIN;
    pfd.fd = _timerFd;

    _isRunning = true;
    while (_isRunning)
    {
        int nready = poll(&pfd, 1, -1); // 4
        if(nready == -1 && errno == EINTR)
        {
            continue;
        }

        else if (nready == -1)
        {
            perror("poll");
            return;
        }
        else if (0 == nready)
        {
            printf("poll timeout!\n");
        }
        else // 定时器到时
        {
            if (pfd.revents & POLLIN)
            {
                handleRead();
                if (_timerCb)
                {
                    _timerCb();
                }
            }
        }
    }
}

void Timer::stop()
{
    _isRunning = false;
    setTimerFd(0, 0);
}

int Timer::createTimerFd()
{
    int fd = timerfd_create(CLOCK_REALTIME, 0);
    CHECK_RET(fd, "timerfd_create");
    return fd;
}

void Timer::setTimerFd(int init, int periodic)
{
    struct itimerspec value;
    value.it_value.tv_sec = init; // 不能设置为 0！
    value.it_value.tv_nsec = 0;
    value.it_interval.tv_sec = periodic;
    value.it_interval.tv_nsec = 0;
    int ret = timerfd_settime(_timerFd, 0, &value, nullptr);
    CHECK_RET(ret, "timerfd_settime");
}

void Timer::handleRead()
{
    uint64_t value; // 可以不设初值
    int ret = read(_timerFd, &value, sizeof(value));
    if (ret != sizeof(value))
    {
        perror("read");
    }
}

} // namespace searchengine
