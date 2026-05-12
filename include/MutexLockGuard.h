#pragma once
#include "MutexLock.h"
#include "NonCopyable.h"

namespace searchengine
{

class MutexLockGuard
:private NonCopyable
{
public:
    MutexLockGuard(MutexLock &);
    ~MutexLockGuard();

private:
    MutexLock &_mutex;
};

}