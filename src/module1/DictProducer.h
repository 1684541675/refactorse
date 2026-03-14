#pragma once
#include <iostream>
#include <bits/stdc++.h>
using namespace std;
class SplitTool;

class DictProducer
{
public:
    DictProducer(const string &dir);
    DictProducer(const string &dir, SplitTool *splitTool);

    void buildEnDict();
    void buildCnDict();
    void buildIndex();
    void storeDict(const char * filepath);
    void storeIndex(const char *filepath);

private:
    void getFiles(string dir);//获取文件绝对路径
    size_t getByteNum_UTF8(const char byte);
    void loadStopWord(string stopDictPath); 

private:
    vector<string> _files;
    vector<string> _enfiles;
    unordered_map<string, int> _dict;//用来写

    map<string, set<int>> _index;
    unordered_set<string> _stopWord;
    string _dir;
    string _endir;
    
    SplitTool * _splitTool;
};
