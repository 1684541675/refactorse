#include "KeyRecommender.h"
#include "Configuration.h"
#include "Dictionary.h"
#include "MyLog.h"
#include "../../3rdparty/json-develop/include/nlohmann/json.hpp"
#include "fifo_map.hpp"
#include <cctype>
#include <unordered_map>
#include <algorithm>
#include <utility>
#include <iostream>
using std::min;
using std::cout;
using std::endl;
using std::unordered_map;
using std::pair;
using namespace nlohmann;
/* 以下为 nlohmann/json 库使用，保证插入顺序不变 */
template <class K, class V, class dummy_compare, class A>
using my_workaround_fifo_map = fifo_map<K, V, fifo_map_compare<K>, A>;
using my_json = basic_json<my_workaround_fifo_map>;
using Json = my_json;

namespace searchengine
{

string KeyRecommender::doQuery(const string &queWord)
{
    // 先判断输入类型
    DictType type; // 枚举类型，用于选择词典
    string queryWord = queWord;
    if (isChineseQuery(queryWord))
    {
        type = DictType::CN;
    }
    else if (isEnglishQuery(queryWord))
    {
        type = DictType::EN;
        queryWord = toLower(queryWord);   // 这里统一转小写
    }
    else
    {
        // 混合输入或非法字符，暂不处理
        LogInfo("\n\tkeyRecommender unsupported query: %s", queryWord.c_str());
        return serializeForNothing();
    }
    Dictionary &pdict = Dictionary::getInstance();
    const unordered_map<string, unordered_set<int>> &indexTable = pdict.getIndexTable(type); // 词典索引

    unordered_set<int> indexId; //获得的索引ID
    string index;
    for (size_t idx = 0; idx < queryWord.length(); ++idx)
    {
        size_t nBytes = nBytesCode(queryWord[idx]);
        index = queryWord.substr(idx, nBytes);
        idx += (nBytes - 1);
        auto it = indexTable.find(index);
        if (it != indexTable.end())
        {
            indexId.insert(it->second.begin(), it->second.end());
        }
    }

    if (indexId.size() == 0)
    {
        LogInfo("\n\tkeyRecommender miss: %s", queryWord.c_str());
        return serializeForNothing();
    }

    priority_queue<MyResult, vector<MyResult>, MyCompare> resultQue;
    statistic(queryWord, type, indexId, resultQue);

    vector<string> result;
    const int candidateCount = stoi(Configuration::getInstance().getConfigMap()["maxkeynum"]);
    for (int i = 0; i < candidateCount; ++i)
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

void KeyRecommender::statistic(const string &queWord,
                               DictType type,
                               const unordered_set<int> &indexId,
                               priority_queue<MyResult, vector<MyResult>, MyCompare> &resultQue)
{
    Dictionary &dictionary = Dictionary::getInstance();
    const vector<pair<string,int>> &dict = dictionary.getDict(type); // vector 类型

    for (auto idx : indexId)
    {
        const string &word = dict[idx].first;
        int freq = dict[idx].second;

        MyResult result(word, freq);
        int dist = distance(word, queWord);
        result.setDist(dist);
        resultQue.push(result);
    }
}

bool KeyRecommender::isEnglishQuery(const string &queryWord)
{
    if (queryWord.empty())
    {
        return false;
    }

    for (unsigned char ch : queryWord)
    {
        if (!isalpha(ch))
        {
            return false;
        }
    }

    return true;
}

bool KeyRecommender::isChineseQuery(const string &queryWord)
{
    if (queryWord.empty())
    {
        return false;
    }

    for (size_t idx = 0; idx < queryWord.size(); ++idx)
    {
        size_t nBytes = nBytesCode(queryWord[idx]);

        if (nBytes != 3)
        {
            return false;
        }

        idx += (nBytes - 1);
    }

    return true;
}

string KeyRecommender::toLower(const string &word)
{
    string res = word;
    for (auto &ch : res)
    {
        ch = tolower(ch);
    }
    return res;
}

size_t KeyRecommender::nBytesCode(const char byte)
{
    int byteNum = 0;

    for(size_t i = 0; i < 6; ++i) 
    {

        if(byte & (1 << (7-i)))
        {
            ++byteNum;
        }       
        else
        {
             break;
        }     
    }

    return byteNum == 0 ? 1 : byteNum;
}

size_t KeyRecommender::length(const string &str)
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
                dp[dist_j]=min({dp[dist_j-1], dp[dist_j], leftup}) + 1;
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


}