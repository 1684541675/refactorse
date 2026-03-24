#include "PageProcesser.h"
#include "WebPage.h"
#include "RssParser.h"
#include "Configuration.h"

#include <chrono>

#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <cstddef>

namespace searchengine
{

using std::ifstream;
using std::string;
using std::vector;
using std::move;
using std::cout;


PageProcesser::PageProcesser(vector<string> &filePathList, vector<WebPage> &pageList)
:_filePathList(filePathList)
,_nonRepetivepageList(pageList)
{
    loadStopWords();
}

void PageProcesser::loadStopWords()
{
    ifstream ifs(Configuration::getInstance().getConfigMap()["stopwords"]);
    if (!ifs)
    {
        perror("can not open stop_words.utf8");
    }
    string word;
    while (ifs >> word)
    {
        _stopWords.insert(word);
    }
}
/**
 *  获取网页库
 *
 *  1. 将网页文件交给 RssParser 解析，生成未去重的 _pageList 对象
 *  2. 使用 simhash 进行网页去重，得到去重后的 _nonRepetivepageList 对象
 *  3. 对 _nonRepetivepageList 对象中每篇文章进行词频统计
 */

void PageProcesser::process()
{
    {
        auto beg = std::chrono::high_resolution_clock::now();
        
        loadPageFromXML(); // 加载网页

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - beg;

        cout << "loadPageFromXML take total "
                << diff.count() << " seconds\n";
       
    }

    {
        auto beg = std::chrono::high_resolution_clock::now();
        cutRedundantPage(); // 网页去重
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - beg;

        cout << "cutRedundantPage take total "
                << diff.count() << " seconds\n";
        

        // 去重结束，可以确定每篇文章的 _docID 与 _doc
        for (size_t idx = 0; idx < _nonRepetivepageList.size(); ++idx)
        {
            _nonRepetivepageList[idx].setPageID(idx);
            _nonRepetivepageList[idx].setPageDoc();
        }
    }

    {
        auto beg = std::chrono::high_resolution_clock::now();
        countFrequence(); // 统计词频
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - beg;
        cout << "countFrequence take total "
                << diff.count() << " seconds\n";
       
    }
}

void PageProcesser::loadPageFromXML()
{
    PageID ID = 0;
    for (auto &filePath : _filePathList)
    {
        RssParser rssParser(filePath);
        for (auto &item : rssParser.getRssItems())
        {
            WebPage page(item);
            page.setPageID(ID++); // 更新 ID 便于去重时排序
            _pageList.push_back(move(page));
            // _pageList.push_back(str); // 存在隐式转换
        }
    }
}

bool cmp(const WebPage &lhs, const WebPage &rhs)
{
    size_t lhs_size = lhs.getContent().size();
    size_t rhs_size = rhs.getContent().size();
    if (lhs_size != rhs_size)
    {
        return lhs_size > rhs_size; // content 长的排前面
    }
    else
    {
        return lhs.getDocId() < rhs.getDocId(); // docid 小的排前面
    }
}

void PageProcesser::cutRedundantPage()
{
    sort(_pageList.begin(), _pageList.end(), cmp); // 将所有文章按 cmp 排序

    for (auto &page : _pageList)
    {
        if (!_comparePages.cut(page)) // 若无需剔除 page 则将其保存到 _nonRepetivepageList 中
        {
            _nonRepetivepageList.push_back(page);
        }
    }
}

// 对每篇文章进行分词并统计词频
void PageProcesser::countFrequence()
{
    for (auto &page : _nonRepetivepageList)
    {
        page.splitWord(_splitTool, _stopWords);
    }
}

}