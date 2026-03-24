#pragma once

#include "tinyxml2.h"
#include <iostream>
#include <vector>
#include <string>

namespace searchengine
{

using std::string;
using std::vector;
using std::cout;
using std::endl;

/*************************************************************
 *
 *  RSS条目
 *
 *************************************************************/
struct RssItem
{
    string title;
    string link;
    string description;
    string content;

    /**
     * true = 有效
     */
    bool check() const
    {
        if (title.empty() || link.empty() || description.empty())
        {
            return false;
        }

        if (title == description)
        {
            return false;
        }

        if (description.size() <= title.size())
        {
            return false;
        }

        bool hasValidChar = false;
        for (auto &s : description)
        {
            if (s != ' ' && s != '\n' && s != '\t')
            {
                hasValidChar = true;
                break;
            }
        }

        return hasValidChar;
    }
};

/*************************************************************
 *
 *  RSS解析器
 *
 *************************************************************/
class RssParser
{
public:
    explicit RssParser(const string &);

    vector<RssItem> &getRssItems();

private:
    void parse(const string &);
    string dissolve(const string &);

private:
    vector<RssItem> _rss;
};

} // namespace searchengine