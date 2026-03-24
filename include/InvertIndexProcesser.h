#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

namespace searchengine
{

using std::vector;
using std::string;
using std::unordered_map;
using std::cout;
using std::endl;

class WebPage;
using PageID = long;

/*************************************************************
 *
 *  倒排索引库生成类
 *
 *************************************************************/
class InvertIndexProcesser
{
public:
    explicit InvertIndexProcesser(vector<WebPage> &, unordered_map<string, unordered_map<PageID, double>> &);

    ~InvertIndexProcesser()
    {
        cout << "~InvertIndexProcesser()" << endl;
    }

    void process();

private:
    vector<WebPage> &_pageList;
    unordered_map<string, unordered_map<PageID, double>> &_invertIndexTable;
    vector<double> _sumOfWeightsPerPage; // 每篇文章中所有单词的 w 的平方和
};

} // namespace searchengine