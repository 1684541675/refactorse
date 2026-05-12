#pragma once
#include "NonCopyable.h"

#include <pthread.h>

namespace searchengine
{
    

class MutexLock
:private NonCopyable
{
public:
    MutexLock();
    ~MutexLock();

    static void check(int,const char *msg);    

    void lock();
    void unlock();
    pthread_mutex_t *getLock();

private:
    pthread_mutex_t _mutex;
};

}
