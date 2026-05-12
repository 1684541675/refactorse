#include "MutexLock.h"

#include <pthread.h>
#include <iostream>
#include <cstring>
using std::cerr;
using std::endl;

namespace searchengine
{

MutexLock::MutexLock()
{
   int ret = pthread_mutex_init(&_mutex, nullptr);
   check(ret, "Failed to initialize mutex");
}

MutexLock::~MutexLock()
{
    int ret = pthread_mutex_destroy(&_mutex);
    check(ret, "Failed to destroy mutex");
}

void MutexLock::check(int ret,const char *msg)
{
    if (ret != 0) 
    {
        cerr << msg << ": "<< strerror(ret) << endl;
        exit(EXIT_FAILURE);
    }
}

void MutexLock::lock()
{
    int ret = pthread_mutex_lock(&_mutex);
    check(ret, "Failed to lock mutex");
}

void MutexLock::unlock()
{
    int ret = pthread_mutex_unlock(&_mutex);
    check(ret, "Failed to unlock mutex");
}

pthread_mutex_t *MutexLock::getLock()
{
    return &_mutex;
}

}