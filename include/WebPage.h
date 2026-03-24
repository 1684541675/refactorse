#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace searchengine
{

using PageID = long;
using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;
class RssItem;
class SplitTool;

/*************************************************************
 *
 *  网页类
 *
 *************************************************************/
class WebPage
{
public:
    explicit WebPage(const string &);
    explicit WebPage(const RssItem &);
    WebPage();
    ~WebPage()
    {

    }

    string getDoc() const;
    PageID getDocId() const;
    string getTitle() const;
    string getUrl() const;
    string getContent() const;
    string getSummary() const;
    int getTotalwords() const;
    unordered_map<string, int> &getWordsMap();

    void setPageID(PageID);
    void setPageDoc();
    void setPageContent(const string &);
    void setPageSummary(const string &);

    void splitWord(SplitTool &, const unordered_set<string> &);


private:
    int _totalWords;
    string _doc; // <doc>...</doc>
    PageID _docID;
    string _docTitle;
    string _docURL;
    string _docContent;
    string _docSummary;                   // 摘要
    unordered_map<string, int> _wordsMap; // 词频集合 <word, freq>
};

}

