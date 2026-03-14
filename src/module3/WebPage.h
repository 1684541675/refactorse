#pragma once

#include <iostream>
#include <bits/stdc++.h>
using namespace std;
using PageID = long; 

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
    unordered_map<string, int> &getWordsMap();

    void setPageID(PageID);
    void setPageDoc();
    void setPageContent(const string &);
    void setPageSummary(const string &);

    void splitWord(SplitTool &, const vector<string> &);


private:
    string _doc; // <doc>...</doc>
    PageID _docID;
    string _docTitle;
    string _docURL;
    string _docContent;
    string _docSummary;                   // 摘要
    unordered_map<string, int> _wordsMap; // 词频集合 <word, freq>
};

