#pragma once
#include "NonCopyable.h"
#include <iostream>
#include <bits/stdc++.h>
using namespace std;
class MutexLock
: NonCopyable
{
public:
    MutexLock();
    ~MutexLock();

    void lock();
    void unlock();
    pthread_mutex_t *getLock();

private:
    pthread_mutex_t _mutex;
};

