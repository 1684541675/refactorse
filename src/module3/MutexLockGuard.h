#pragma once
#include "MutexLock.h"

class MutexLockGuard
{
public:
    MutexLockGuard(MutexLock &);
    ~MutexLockGuard();

private:
    MutexLock &_mutex;
};

