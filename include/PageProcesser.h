#pragma once

#include "CompareSimhash.h"
#include "SplitTool.h"

#include <vector>
#include <unordered_set>
#include <string>
#include <iostream>

namespace searchengine
{

using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::unordered_set;

class WebPage;

/*************************************************************
 *
 *  网页库生成类
 *
 *************************************************************/
class PageProcesser
{
public:
    explicit PageProcesser(vector<string> &, vector<WebPage> &);

    ~PageProcesser()
    {
        cout << "~PageProcesser()" << endl;
    }

    void process();

private:
    void loadStopWords();

    void loadPageFromXML();  // 加载网页
    void cutRedundantPage(); // 网页去重
    void countFrequence();   // 统计词频

private:
    vector<string> &_filePathList;
    vector<WebPage> &_nonRepetivepageList;

    vector<WebPage> _pageList;
    unordered_set<string> _stopWords;

    CompareSimhash _comparePages; // 网页比较器
    SplitTool _splitTool;         // 分词器
};

} // namespace searchengine