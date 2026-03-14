#pragma once

#include "../3rdparty/simhash-cppjieba/cppjieba/Jieba.hpp"
#include <string>
#include <vector>

namespace searchengine {

using std::string;
using std::vector;

class SplitTool
{
public:
    SplitTool();

    vector<string> cut(const string &sentence);

private:
    cppjieba::Jieba _jieba;
};

}