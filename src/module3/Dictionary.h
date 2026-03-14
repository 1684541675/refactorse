#pragma once 
#include <iostream>
#include <bits/stdc++.h>
using namespace std;
class Dictionary
{
public:
    static Dictionary *getInstance();
    static void destory();

    const vector<pair<string, int>> &getDict();
    const map<string, set<int>> &getIndexTable();

private:
    void initDict();
    void initIndex();
private:
    Dictionary()
    {

    }; //单例类
    
    ~Dictionary()
    {

    };
    static Dictionary *_singletonDict;
    vector<pair<string, int>> _dict;
    map<string, set<int>> _index; //分别加载词典文件与索引文件
};

