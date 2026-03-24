#include "PageLib.h"
#include "Configuration.h"

#include <fstream>
#include <iostream>


namespace searchengine
{

using std::string;
using std::ofstream;
using std::cout;
using std::endl;

PageLib::PageLib(const string &dirPath)
: _dirScanner(dirPath)
, _pageProcesser(_dirScanner.getFilePathList(), _pageList)
, _invertIndexProcesser(_pageList, _invertIndexTable)
, _offsetProcesser(_pageList, _offsetTable)
{

}

void PageLib::create()
{
    _pageProcesser.process();        // 生成网页库
    _invertIndexProcesser.process(); // 生成倒排索引库
    _offsetProcesser.process();      // 生成偏移库
}

void PageLib::store()
{
    auto &config = Configuration::getInstance().getConfigMap();

    string ripepagePath = config["ripepage"];
    string InvertIndex = config["invertIndex"];
    string offsetPath = config["offset"];

    // 写网页库
    ofstream ofs1(ripepagePath);
    if (!ofs1)
    {
        cout << "can not open ripepage.dat" <<endl;
        return;
    }

    PageID ID = 0;
    for (auto &page : _pageList)
    {
        ofs1 << page.getDoc();
    }

    cout << "store ripepage successfully" << endl;

    // 写倒排索引库
    ofstream ofs2(InvertIndex);
    if (!ofs2)
    {
        cout<< "can not open invertIndex.dat" <<endl;
        return;
    }

    for (auto &wordPair : _invertIndexTable)
    {
        ofs2 << wordPair.first << " ";
        for (auto &pagePair : wordPair.second)
        {
            ofs2 << pagePair.first << " "
                 << pagePair.second << " ";
        }
        ofs2 << endl;
    }

    cout << "store InvertIndex successfully" << endl;

    // 写偏移库
    ofstream ofs3(offsetPath);
    if (!ofs3)
    {
        cout << "can not open offset.dat" <<endl;
        return;
    }

    for (size_t idx = 0; idx < _offsetTable.size(); ++idx)
    {
        ofs3 << _pageList[idx].getDocId() << " "
             << _offsetTable[idx].first << " "
             << _offsetTable[idx].second << " "
             << endl;
    }

    cout << "store offset successfully" << endl;
}

} // namespace searchengine