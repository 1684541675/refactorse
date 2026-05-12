#include "Condition.h"
#include <iostream>
#include <cstring>  
#include <cstdlib>
using std::cerr;
using std::endl;

namespace searchengine
{

Condition::Condition(MutexLock &mutex)
:_mutex(mutex)
{
    int ret = pthread_cond_init(&_cond, nullptr);
    check(ret, "pthread_cond_init");
}

Condition::~Condition()
{
    int ret = pthread_cond_destroy(&_cond);
    check(ret, "pthread_cond_destroy");
}

void  Condition::check(int ret, const char *msg)
{
    if (ret != 0) {
        cerr << msg << " failed: "
                  << strerror(ret) << endl;
        exit(EXIT_FAILURE);
    }
}

void Condition::wait()
{
    int ret = pthread_cond_wait(&_cond, _mutex.getLock());
    check(ret, "pthread_cond_wait");
}

void Condition::notify()
{
    int ret = pthread_cond_signal(&_cond);
    check(ret, "pthread_cond_signal");
}

void Condition::notifyAll()
{
    int ret = pthread_cond_broadcast(&_cond);
    check(ret, "pthread_cond_broadcast");
}
   
}
