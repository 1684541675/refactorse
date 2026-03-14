#pragma once

#include "Dictionary.h"
#include <iostream>
#include <bits/stdc++.h>
using namespace std;
class MyResult
{
public:
    MyResult(string word, int freq, int dist = 999)
    :_word(word)
    ,_freq(freq)
    ,_dist(dist)
    {

    }
    string getWord() const
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
    string _word; //查询到的单词
    int _freq;    //单词频率
    int _dist;    //编辑距离
};

struct MyCompare
{
    //这里使用的是堆排序
    bool operator()(const MyResult &lhs, const MyResult &rhs)
    {
        if (lhs.getDist() > rhs.getDist()) //编辑距离小的排前面
        {
            return true;
        }
        else if (lhs.getDist() == rhs.getDist())
        {
            if (lhs.getFreq() < rhs.getFreq()) //频率大的排前面
            {
                return true;
            }
            else if (lhs.getFreq() == rhs.getFreq())
            {
                if (lhs.getWord() > rhs.getWord()) //字典序排序
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
    string doQuery(const string &); //执行查询

private:
    void statistic(const string &queryWord, set<int> &, priority_queue<MyResult, vector<MyResult>, MyCompare> &resultQue); //进行计算
    size_t nBytesCode(const char ch);
    size_t length(const string &str);
    int distance(const string &, const string &rhs); //计算最小编辑距离

    string serializeForNothing();
    string serialize(const vector<string> &);
};

