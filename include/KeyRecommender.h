#pragma once

#include <string>
#include <unordered_set>
#include <queue>
#include <vector>

using std::string;
using std::unordered_set;
using std::priority_queue;
using std::vector;

namespace searchengine
{

class MyResult
{
public:
    MyResult(const string &word, int freq, int dist = 999)
    : _word(word)
    , _freq(freq)
    , _dist(dist)
    {

    }

    const string &getWord() const
    {
        return _word;
    }

    int getFreq() const
    {
        return _freq;
    }

    int getDist() const
    {
        return _dist;
    }

    void setDist(int dist)
    {
        _dist = dist;
    }

private:
    string _word; // 查询到的单词
    int _freq;    // 单词频率
    int _dist;    // 编辑距离
};

struct MyCompare
{
    // priority_queue 底层使用堆结构维护候选结果
    bool operator()(const MyResult &lhs, const MyResult &rhs) const
    {
        if (lhs.getDist() > rhs.getDist()) // 编辑距离小的排前面
        {
            return true;
        }
        else if (lhs.getDist() == rhs.getDist())
        {
            if (lhs.getFreq() < rhs.getFreq()) // 频率大的排前面
            {
                return true;
            }
            else if (lhs.getFreq() == rhs.getFreq())
            {
                if (lhs.getWord() > rhs.getWord()) // 字典序小的排前面
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
};

class KeyRecommender
{
public:
    KeyRecommender() = default;
    ~KeyRecommender() = default;

    string doQuery(const string &queryWord); // 执行查询

private:
    void statistic(const string &queryWord,
                   DictType type,
                   const unordered_set<int> &candidateSet,
                   priority_queue<MyResult, vector<MyResult>, MyCompare> &resultQue);

    bool isEnglishQuery(const string &queryWord);
    bool isChineseQuery(const string &queryWord);

    string toLower(const string &word);

    size_t nBytesCode(const char ch);
    size_t length(const string &str);
    int distance(const string &lhs, const string &rhs); // 计算最小编辑距离

    string serializeForNothing();
    string serialize(const vector<string> &resultVec);
};

}