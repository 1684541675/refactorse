#include "PageLib.h"
#include "Configuration.h"

PageLib::PageLib(const string &dirPath)
:_dirScanner(dirPath)
,_pageProcesser(_dirScanner.getFilePathList(), _pageList)
,_invertIndexProcesser(_pageList, _invertIndexTable)
,_offsetProcesser(_pageList, _offsetTable)
{

}

void PageLib::create()
{
    _pageProcesser.process(); // 生成网页库
    _invertIndexProcesser.process(); // 生成倒排索引库
    _offsetProcesser.process(); // 生成偏移库
}

void PageLib::store()
{
    string ripepagePath = Configuration::getInstance()->getConfigMap()["ripepage"];
    string InvertIndex = Configuration::getInstance()->getConfigMap()["invertIndex"];
    string offsetPath = Configuration::getInstance()->getConfigMap()["offset"];

    // 写网页库
    ofstream ofs1(ripepagePath);
    if (!ofs1)
    {
        perror("can not open ripepage.dat");
        exit(EXIT_FAILURE);
    }
    PageID ID = 0;
    for (auto &page : _pageList)
    {
        page.setPageID(ID++);
        page.setPageDoc();
        ofs1 << page.getDoc();
    }
    ofs1.close();
    cout<<"store ripepage successfully"<<endl;
    // 写倒排索引库
    ofstream ofs2(InvertIndex);
    if (!ofs2)
    {
        perror("can not open invertIndex.dat");
        exit(EXIT_FAILURE);
    }
    for (auto &wordPair : _invertIndexTable)
    {
        ofs2 << wordPair.first << " ";
        for (auto &pagePair : wordPair.second)
        {
            ofs2 << pagePair.first << " "
                 << pagePair.second << " ";
        }
        ofs2 <<endl;
    }
    ofs2.close();
    cout<<"store InvertIndex successfully"<<endl;
    // 写偏移库
    ofstream ofs3(offsetPath);
    if (!ofs3)
    {
        perror("can not open offset.dat");
        exit(EXIT_FAILURE);
    }
    for (size_t idx = 0; idx < _offsetTable.size(); ++idx)
    {
        ofs3 << _pageList[idx].getDocId() << " "
             << _offsetTable[idx].first << " "
             << _offsetTable[idx].second << " "
             << endl;
    }
    ofs3.close();

    cout << "store offset successfully" << endl;
}
