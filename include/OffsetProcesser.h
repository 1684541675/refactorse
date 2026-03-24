#pragma once

#include <iostream>
#include <vector>
#include <utility>

namespace searchengine
{

using std::vector;
using std::pair;
using std::cout;
using std::endl;

class WebPage;

/*************************************************************
 *
 *  网页偏移库生成类
 *
 *************************************************************/
class OffsetProcesser
{
public:
    explicit OffsetProcesser(vector<WebPage> &, vector<pair<size_t, size_t>> &); // 参数传入一个网页库

    ~OffsetProcesser()
    {
        cout << "~OffsetProcesser()" << endl;
    }

    void process(); // 生成偏移库

private:
    vector<WebPage> &_pagelist;
    vector<pair<size_t, size_t>> &_offsetlib;
};

} // namespace searchengine