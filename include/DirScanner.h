#pragma once

#include <string>
#include <vector>
#include <iostream>

namespace searchengine
{

using std::string;
using std::vector;
using std::cout;
using std::endl;

/*************************************************************
 *
 *  目录扫描类
 *
 *************************************************************/
class DirScanner
{
public:
    explicit DirScanner(const string &);

    ~DirScanner()
    {
        cout << "~DirScanner()" << endl;
    }

    void traverse();

    vector<string> &getFilePathList();

private:
    void traverse(const string &);

private:
    vector<string> _filePathList; // _dirPath 目录下所有文件的路径
    string _dirPath;
};

} // namespace searchengine