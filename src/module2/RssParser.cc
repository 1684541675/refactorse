#include "RssParser.h"

RssPraser::RssPraser(const char *filePath)
{
    prase(filePath);
}

void RssPraser::prase(const char *filename)
{
    cout << "RssPraser is prasing " << filename << endl;

    XMLDocument doc;
    if (doc.LoadFile(filename))
    {
        doc.PrintError();
        exit(EXIT_FAILURE);
    }

    XMLElement *channel = doc.FirstChildElement("rss")->FirstChildElement("channel");
    RssItem home; // 首页内容
    home.title = channel->FirstChildElement("title")->GetText();
    home.link = channel->FirstChildElement("link")->GetText();
    home.description = channel->FirstChildElement("description")->GetText();
    if (!home.check()) // 检查 item，若返回 false 则将其保留，否则忽略
    {
        _rss.push_back(home);
    }

    XMLElement *item = channel->FirstChildElement("item");
    while (item)
    {
        RssItem page; // 存放 item 的内容
        page.title = item->FirstChildElement("title")->GetText();
        page.link = item->FirstChildElement("link")->GetText();
        page.description="";
        if(item->FirstChildElement("description"))
        {
            page.description = item->FirstChildElement("description")->GetText();
        }
        // page.content = item->FirstChildElement("content")->GetText();

        // 析出标签
        page.description = dissolve(page.description);
        // page.content = dissolve(page.content);

        if (!page.check())        // 检查 item，若返回 false 则将其保留，否则忽略
        {
            _rss.push_back(page); // 插入 _rss
        }

        // 获取下一个 item
        item = item->NextSiblingElement("item");
    }
}

vector<RssItem> &RssPraser::getRssItems()
{
    return _rss;
}


string RssPraser::dissolve(string text)
{
    string res = text;
    {
        regex pattern("<(.[^>]*)>");
        res = regex_replace(res, pattern, "");
    }
    {
        regex pattern("&nbsp;");
        res = regex_replace(res, pattern, "");
    }
    return res;
}
