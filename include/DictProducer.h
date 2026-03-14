#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace searchengine {

using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;

class SplitTool;

class DictProducer
{
public:
    // 英文
    DictProducer(const string &dir);

    // 中文
    DictProducer(const string &dir, SplitTool *splitTool);

    void buildEnDict();
    void buildCnDict();
    void buildIndex();

    void storeDict(const char *filepath);
    void storeIndex(const char *filepath);

private:
    void getFiles(const string &dir); // 获取文件路径
    size_t getByteNum_UTF8(char byte);
    void loadStopWord(const string &stopDictPath);

private:
    vector<string> _files;

    // word -> freq
    unordered_map<string, int> _dict;

    // character -> word index set
    unordered_map<string, unordered_set<int>> _index;

    unordered_set<string> _stopWord;

    string _dir;
    string _endir;

    SplitTool *_splitTool;
};

} // namespace searchengine