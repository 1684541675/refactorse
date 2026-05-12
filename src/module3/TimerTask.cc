#include "TimerTask.h"
#include "CacheManager.h"

namespace searchengine
{

void TimerTask::process()
{
    CacheManager::getInstance().sync(); // 同步缓存
}

}