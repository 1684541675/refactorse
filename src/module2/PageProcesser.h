#pragma once
#include "CompareSimhash.h"
#include "SplitTool.h"

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
    vector<string> _stopWords;
    CompareSimhash _comparePages; // 网页比较器
    SplitTool _splitTool;         // 分词器
};

