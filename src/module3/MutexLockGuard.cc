#include "MutexLockGuard.h"

namespace searchengine
{

MutexLockGuard::MutexLockGuard(MutexLock &mutex)
:_mutex(mutex)
{
    _mutex.lock();
}

MutexLockGuard::~MutexLockGuard()
{
    _mutex.unlock();
}
 
}
