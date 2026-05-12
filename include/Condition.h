#pragma once
#include "MutexLock.h"
#include "NonCopyable.h"
#include <pthread.h>

namespace searchengine
{

class Condition
:private NonCopyable
{
public:
    Condition(MutexLock &);
    ~Condition();

    static void check(int,const char *msg);

    void wait();
    void notify();
    void notifyAll();

private:
    MutexLock &_mutex;
    pthread_cond_t _cond;
};

}
