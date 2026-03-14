#include "CacheManager.h"
#include "Configuration.h"


CacheManager *CacheManager::_pInstance = CacheManager::getInstance(); // 饿汉

CacheManager *CacheManager::getInstance()
{
    if (_pInstance == nullptr)
    {
        _pInstance = new CacheManager();
        atexit(destroy);
    }
    return _pInstance;
}

CacheGroup &CacheManager::getCacheGroup(size_t idx)
{
    return _caches[idx];
}

CacheManager::CacheManager()
:_cacheNums(stoul(Configuration::getInstance()->getConfigMap()["workernum"]))
,_maxRecord(stoul(Configuration::getInstance()->getConfigMap()["recordnum"]))
,_caches(_cacheNums, _maxRecord) // 创建 _cacheNums 个 cache group 对象（这里在构造对象时还要传入 _maxRecord）
{

}

void CacheManager::destroy()
{
    cout << "void CacheManager::destroy()" << endl;
    if (_pInstance)
    {
        delete _pInstance;
        _pInstance = nullptr;
    }
}

void CacheManager::sync()
{
    cout << "timer thread: start sync" << endl;

    auto &first_group = _caches[0];
    first_group._onlyRead = true;
    for (auto &group : _caches)
    {
        auto &pendingCache = group._pendingUpdateCache;
#ifdef __DEBUG__
        cout << "group._pengdingCache.size() = " << pendingCache.size() << endl;
#endif
        for (auto &record : pendingCache._resultList)
        {
            first_group.insertRecord(record.first, record.second);
#ifdef __DEBUG__
            cout << "first_group._mainCache.size() = " << first_group._mainCache.size() << endl;
#endif
        }
        pendingCache.clear();
    }


    for (auto &group : _caches)
    {
        group._onlyRead = true;
        group.update(first_group);
        group._onlyRead = false;
    }
    first_group._onlyRead = false;

#ifdef __DEBUG__
    printf("\t(File:%s, Func:%s(), Line:%d)\n", __FILE__, __FUNCTION__, __LINE__);
    cout << "timer thread: end sync" << endl;
#endif
}

