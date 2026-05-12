#pragma once

#include "WebPage.h"
#include "SplitTool.h"

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <set>
using std::string;
using std::unordered_map;
using std::vector;
using std::set;
using std::unordered_set;

namespace searchengine
{

class WebPageSearcher
{
public:
    WebPageSearcher();
    ~WebPageSearcher() = default;

    string doQuery(string);

private:
    void loadFromFile();

    unordered_map<string, double> getVectorX(WebPage &);
    unordered_set<PageID> getIDs(WebPage &);
    vector<PageID> getSortedIDs(const unordered_map<string, double> &, const unordered_set<PageID> &);

    void setSummarys(vector<PageID> &, WebPage &);

    string serializeForNothing();
    string serialize(const vector<PageID> &);

private:
    vector<WebPage> _pageList;
    // unordered_map<int, pair<int, int>> _offsetTable; // 感觉不需要一直存放在内存
    unordered_map<string, unordered_map<PageID, double>> _invertIndexTable;

    SplitTool _splitTool;
    unordered_set<string> _stopWords;
};

}