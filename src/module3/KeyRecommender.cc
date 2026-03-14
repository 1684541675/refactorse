#include "KeyRecommender.h"
#include "Configuration.h"
#include "MyLog.h"
#include "../../3rdparty/json-develop/include/nlohmann/json.hpp"
#include "fifo_map.hpp"
using namespace nlohmann;
/* 以下为 nlohmann/json 库使用，保证插入顺序不变 */
template <class K, class V, class dummy_compare, class A>
using my_workaround_fifo_map = fifo_map<K, V, fifo_map_compare<K>, A>;
using my_json = basic_json<my_workaround_fifo_map>;
using Json = my_json;


string KeyRecommender::doQuery(const string &queWord)
{
    Dictionary *pdict = Dictionary::getInstance();
    const map<string, set<int>> &indexTable = pdict->getIndexTable(); // 词典索引

    set<int> indexId; //获得的索引ID
    string index;
    for (size_t idx = 0; idx < queWord.length(); ++idx)
    {
        size_t nBytes = nBytesCode(queWord[idx]);
        index = queWord.substr(idx, nBytes);
        idx += (nBytes - 1);
        auto it = indexTable.find(index);
        if (it != indexTable.end())
        {
            indexId.insert(it->second.begin(), it->second.end());
        }
    }

    if (indexId.size() == 0)
    {
        LogInfo("\n\tkeyRecommender miss: %s", queWord.c_str());
        return serializeForNothing();
    }

    priority_queue<MyResult, vector<MyResult>, MyCompare> resultQue;
    statistic(queWord, indexId, resultQue);

    vector<string> result;
    const int candicateCount = stoi(Configuration::getInstance()->getConfigMap()["maxkeynum"]);
    for (int i = 0; i < candicateCount; ++i)
    {
        if (!resultQue.empty())
        {
#ifdef __DEBUG__
            cout << resultQue.top().getWord() << endl;
#endif
            result.push_back(resultQue.top().getWord());
            resultQue.pop();
        }
    }

    return serialize(result);
}

void KeyRecommender::statistic(const string &queWord,set<int> &indexId,
                               priority_queue<MyResult, vector<MyResult>, MyCompare> &resultQue)
{
    Dictionary *pdict = Dictionary::getInstance();
    const vector<pair<string, int>> &dict = pdict->getDict(); // 词典

    for (auto it = indexId.begin(); it != indexId.end(); ++it)
    {
        MyResult result(dict[*it].first, dict[*it].second);
        int dist = distance(dict[*it].first,queWord);
        result.setDist(dist);
        resultQue.push(result);
    }
}

size_t KeyRecommender::nBytesCode(const char ch)
{
    if (ch & (1 << 7))
    {
        int nBytes = 1;
        for (int idx = 0; idx != 6; ++idx)
        {
            if (ch & (1 << (6 - idx)))
            {
                ++nBytes;
            }
            else
            {
                break;
            }
        }
        return nBytes;
    }
    return 1;
}

size_t KeyRecommender::length(const std::string &str)
{
    size_t ilen = 0;
    for (size_t idx = 0; idx != str.size(); ++idx)
    {
        int nBytes = nBytesCode(str[idx]);
        idx += (nBytes - 1);
        ++ilen;
    }
    return ilen;
}

int KeyRecommender::distance(const string &lhs, const string &rhs)
{
    if (lhs == rhs) //相等返回0
    {
        return 0;
    }

    //计算最小编辑距离-包括处理中英文
    size_t lhs_len = length(lhs);
    size_t rhs_len = length(rhs);
    vector<int> dp(rhs_len+1,0);
    for (size_t idx = 1; idx <= rhs_len; ++idx)
    {
        dp[idx] = idx;
    }

    string sublhs, subrhs;

    for (size_t dist_i = 1, lhs_idx = 0; dist_i <= lhs_len; ++dist_i, ++lhs_idx)
    {
        size_t nBytes = nBytesCode(lhs[lhs_idx]);
        sublhs = lhs.substr(lhs_idx, nBytes);
        lhs_idx += (nBytes - 1);
        int leftup=dp[0];
        dp[0]=dist_i;
        for (size_t dist_j = 1, rhs_idx = 0; dist_j <= rhs_len; ++dist_j, ++rhs_idx)
        {
            nBytes = nBytesCode(rhs[rhs_idx]);
            subrhs = rhs.substr(rhs_idx, nBytes);
            rhs_idx += (nBytes - 1);
            int temp=dp[dist_j];
            if (sublhs == subrhs)
            {
                dp[dist_j]=leftup;
            }
            else
            {
                dp[dist_j]=min(min(dp[dist_j-1],dp[dist_j]),leftup)+1;
            }
            leftup=temp;
        }
    }
    return dp[rhs_len];
}
/**
 *  未找到结果，返回 404 序列化结果
 */
string KeyRecommender::serializeForNothing()
{
    Json root;
    root["msgID"] = 404;
    root["msg"] = json("未能找到相关关键词");
    // root["msg"] = "未能找到相关关键词"; // 一样的效果

    return root.dump(4);// 序列化为带4空格缩进的JSON字符串
}

/**
 *  找到结果，返回所有关键词的序列化结果
 */
string KeyRecommender::serialize(const vector<string> &result)
{
    Json root;
    root["msgID"] = 100;

    Json msg = result;
    root["msg"] = msg;

#ifdef __DEBUG__
    printf("\t(File:%s, Func:%s(), Line:%d)\n", __FILE__, __FUNCTION__, __LINE__);
    cout << root.dump(4) << endl;
#endif

    return root.dump(4);
}
