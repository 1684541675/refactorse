#pragma once
#include "../../3rdparty/simhash-cppjieba/cppjieba/Jieba.hpp"
#include <iostream>
#include <bits/stdc++.h>
using namespace std;
using namespace cppjieba;

/*************************************************************
 *
 *  分词工具类
 *
 *************************************************************/
class SplitTool
{
    const string DICT_PATH = "../../3rdparty/simhash-cppjieba/dict/jieba.dict.utf8";
    const string HMM_PATH = "../../3rdparty/simhash-cppjieba/dict/hmm_model.utf8";
    const string USER_DICT_PATH = "../../3rdparty/simhash-cppjieba/dict/user.dict.utf8";

public:
    SplitTool();

    vector<string> cut(const string &);

private:
    Jieba _jieba;
};

