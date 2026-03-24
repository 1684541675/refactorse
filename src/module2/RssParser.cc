#include "RssParser.h"

#include <regex>

namespace searchengine
{

using std::regex;
using std::regex_replace;

using namespace tinyxml2; // 只在 .cc 使用

RssParser::RssParser(const string &filePath)
{
    parse(filePath);
}

void RssParser::parse(const string &filename)
{
    cout << "RssParser is parsing " << filename << endl;

    XMLDocument doc;
    if (doc.LoadFile(filename.c_str()))
    {
        doc.PrintError();
        exit(EXIT_FAILURE);
    }

    XMLElement *rss = doc.FirstChildElement("rss");
    if (!rss) return;

    XMLElement *channel = rss->FirstChildElement("channel");
    if (!channel) return;

    // 安全取值函数
    auto getText = [](XMLElement *parent, const char *tag) -> string {
        if (!parent) return "";
        XMLElement *elem = parent->FirstChildElement(tag);
        if (elem && elem->GetText())
            return elem->GetText();
        return "";
    };

    XMLElement *item = channel->FirstChildElement("item");

    while (item)
    {
        RssItem page;

        page.title = getText(item, "title");
        page.link  = getText(item, "link");
        page.description = getText(item, "description");

        // 去HTML标签
        page.description = dissolve(page.description);

        // ✔ 新语义：true = 有效
        if (page.check())
        {
            _rss.push_back(page);
        }

        item = item->NextSiblingElement("item");
    }
}

vector<RssItem> &RssParser::getRssItems()
{
    return _rss;
}

string RssParser::dissolve(const string &text)
{
    static regex tagPattern("<[^>]*>");
    static regex nbspPattern("&nbsp;");

    string res = regex_replace(text, tagPattern, "");
    res = regex_replace(res, nbspPattern, " ");

    return res;
}

} // namespace searchengine