#pragma once 

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;
using std::pair;

namespace searchengine
{

enum class DictType
{
    CN,
    EN
};

class Dictionary
{
public:
    static Dictionary &getInstance();

    const vector<pair<string,int>> &getDict(DictType) const;
    const unordered_map<string, unordered_set<int>> &getIndexTable(DictType) const;

private:
    Dictionary();
    ~Dictionary() {}

    Dictionary(const Dictionary &) = delete;
    Dictionary &operator=(const Dictionary &) = delete;

    void initDict(const string &dictPath, vector<pair<string,int>> &dict);
    void initIndex(const string &indexPath, unordered_map<string, unordered_set<int>> &index);

private:
    // 中文词典和中文索引
    vector<pair<string,int>> _cnDict;
    unordered_map<string, unordered_set<int>> _cnIndex;

    // 英文词典和英文索引
    vector<pair<string,int>> _enDict;
    unordered_map<string, unordered_set<int>> _enIndex;
};

}