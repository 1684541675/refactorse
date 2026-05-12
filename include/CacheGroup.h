#pragma once
#include "LRUCache.h"
#include <string>
using std::string;

namespace searchengine
{

class CacheManager;
/*************************************************************
 *
 *  cache group 类（包含两块 cache）
 *
 *************************************************************/
class CacheGroup
{
    friend class CacheManager;

public:
    CacheGroup(size_t);

    string getRecord(const string &);
    void insertRecord(const string &, const string &);
    void update(const CacheGroup &);

private:
    LRUCache _mainCache;          // 主 cache
    LRUCache _pendingUpdateCache; // 更新 cache（存放自上次更新以来新的记录，用于更新操作）
    bool _onlyRead; // 若为 true，则新记录只写入主 cache，不写入待更新 cache
};

}