#pragma once
#include "NonCopyable.h"
#include <pthread.h>
#include <functional>

using std::function;

namespace searchengine
{

class Thread
:private NonCopyable
{
private:
    using ThreadCallBack = function<void()>;

public:
    Thread(ThreadCallBack &&);
    Thread(size_t, ThreadCallBack &&);
    virtual ~Thread();

    void create(); // 开启子线程（pthread_create -> threadFunc）
    void join();   // 回收子线程（pthread_join）

private:
    static void *threadFunc(void *); // 线程的入口函数

private:
    size_t _id;
    pthread_t _thid;
    bool _isRunning;
    ThreadCallBack _cb; // 类似 run（这里会传入 ThreadPool::doTask）
};

}