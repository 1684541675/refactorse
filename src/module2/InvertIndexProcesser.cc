#include "InvertIndexProcesser.h"
#include "WebPage.h"


InvertIndexProcesser::InvertIndexProcesser(vector<WebPage> &pageList,unordered_map<string, unordered_map<PageID, double>> &invertIndexTable)
:_pageList(pageList)
,_invertIndexTable(invertIndexTable)
{

}

/**
 *  生成倒排索引
 */
void InvertIndexProcesser::process()
{

    _sumOfWeightsPerPage.resize(_pageList.size(), 0.0); // 为 _sumOfWeightsPerPage 申请内存并初始化

    for (auto &page : _pageList) // WebPage page
    {
        auto &wordsMap = page.getWordsMap(); // unordered_map<string, int> wordsMap
        for (auto &wordPair : wordsMap)      // pair<string, int> wordPair
        {
            string word = wordPair.first;

            int wordNumInPage = wordsMap.size(); // page 网页中的单词总数
            if (wordNumInPage < 1)
            {
                perror("this page contains no word\n");
                exit(EXIT_FAILURE);
            }
            double TF = (double)wordPair.second / wordNumInPage;

            _invertIndexTable[word].insert({page.getDocId(), TF}); // value = TF
        }
    }
    // 遍历所有单词 word
    for (auto &invertIndexPair : _invertIndexTable) // pair<string, unordered_map<int, double>> invertIndexPair
    {
        // 遍历 word 所在的所有文章 pageId
        auto &pageIdMap = invertIndexPair.second; // unordered_map<int, double> pageIdMap
        for (auto &pageIdPair : pageIdMap)        // pair<int, double> pageIdPair
        {
            int pageId = pageIdPair.first;
            double TF = pageIdPair.second;
            int DF = pageIdMap.size(); // 上限 -> N
            int N = _pageList.size();
            double IDF = 0.0;
            if (N != DF)
            {
                IDF = (double)log10((double)N / (DF + 1));
            }
            double w = TF * IDF;

            pageIdPair.second = w; // value = w
            _sumOfWeightsPerPage[pageId] += w * w;
        }
    }

    for (auto &invertIndexPair : _invertIndexTable) // pair<string, unordered_map<int, double>> invertIndexPair
    {
        auto &pageIdMap = invertIndexPair.second; // unordered_map<int, double> pageIdMap
        for (auto &pageIdPair : pageIdMap)        // pair<int, double> pageIdPair
        {
            int pageId = pageIdPair.first;
            double sumWeight = _sumOfWeightsPerPage[pageId];
            if (sumWeight == 0.0)
            {
                perror("this page's sumWeight equal 0.0\n");
                exit(EXIT_FAILURE);
            }
            pageIdPair.second /= sqrt(sumWeight); // value = w'
        }
    }

}

