#include "CompareSimhash.h"
#include "WebPage.h"
#include "Configuration.h"

namespace searchengine
{

using namespace simhash; // 只在 .cc 使用

CompareSimhash::CompareSimhash()
: _simhasher(
    Configuration::getInstance().getConfigMap()["jieba_dict"],
    Configuration::getInstance().getConfigMap()["jieba_hmm"],
    Configuration::getInstance().getConfigMap()["jieba_idf"],
    Configuration::getInstance().getConfigMap()["stopwords"]
)
{

}

/**
 *	若剔除网页 page 则返回 true，否则返回 false
 *
 *  1. 输入 page 的引用
 *  2. 求得 page 的 hash 值
 *  3. 判断是否需要剔除该网页
 */
bool CompareSimhash::cut(const WebPage &page)
{
    uint64_t i = 0;
    _simhasher.make(page.getContent(), topN, i);

    uint64_t a = 0xff000000;
    uint64_t b = 0x00ff0000;
    uint64_t c = 0x0000ff00;
    uint64_t d = 0x000000ff;

    uint64_t keyA = i & a;
    uint64_t keyB = i & b;
    uint64_t keyC = i & c;
    uint64_t keyD = i & d;

    // ===== 查找阶段 =====
    if (_bucketA.count(keyA))
    {
        for (auto &hash : _bucketA[keyA])
        {
            if (Simhasher::isEqual(i, hash))
            {
                return true;
            }  
        }
    }

    if (_bucketB.count(keyB))
    {
        for (auto &hash : _bucketB[keyB])
        {
            if (Simhasher::isEqual(i, hash))
            {
                return true;
            }
        }
    }

    if (_bucketC.count(keyC))
    {
        for (auto &hash : _bucketC[keyC])
        {
            if (Simhasher::isEqual(i, hash))
            {
                return true;
            }
        }
    }

    if (_bucketD.count(keyD))
    {
        for (auto &hash : _bucketD[keyD])
        {
            if (Simhasher::isEqual(i, hash))
            {
                return true;
            }
        }
    }

    // ===== 插入阶段 =====
    _bucketA[keyA].push_back(i);
    _bucketB[keyB].push_back(i);
    _bucketC[keyC].push_back(i);
    _bucketD[keyD].push_back(i);

    return false;
}

} // namespace searchengine