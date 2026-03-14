#pragma once
#include "DirScanner.h"
#include "PageProcesser.h"
#include "InvertIndexProcesser.h"
#include "OffsetProcesser.h"
#include "WebPage.h"
/*************************************************************
 *
 *  网页库类
 *
 *  1. 包含三个网页库数据
 *  2. 包含三个网页库生成类对象
 *
 *************************************************************/
class PageLib
{
public:
    explicit PageLib(const string &);
    ~PageLib()
    {
        cout << "~PageLib()" << endl;
    }

    void create();
    void store();

private:

    DirScanner _dirScanner;
    PageProcesser _pageProcesser;
    InvertIndexProcesser _invertIndexProcesser;
    OffsetProcesser _offsetProcesser;
    vector<WebPage> _pageList;                                              // 网页库
    vector<pair<size_t, size_t>> _offsetTable;                              // 网页偏移库
    unordered_map<string, unordered_map<PageID, double>> _invertIndexTable; // 倒排索引库
};

