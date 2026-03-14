#include "TimerTask.h"
#include "CacheManager.h"

#include <iostream>
#include <bits/stdc++.h>
using namespace std;

void TimerTask::process()
{
    CacheManager::getInstance()->sync(); // 同步缓存
}

