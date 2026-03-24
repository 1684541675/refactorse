#include "DirScanner.h"

#include <dirent.h>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>

namespace searchengine
{

using std::string;
using std::vector;
using std::cout;
using std::endl;

DirScanner::DirScanner(const string &dirPath)
: _dirPath(dirPath)
{
    cout << "DirScanner()" << endl;
    traverse();

    for (auto &item : _filePathList)
    {
        cout << item << endl;
    }
}

/**
 *  打开 _dirPath 目录，递归获取其中所有文件的路径信息
 */
void DirScanner::traverse()
{
    traverse(_dirPath);
}

void DirScanner::traverse(const string &dirName)
{
    DIR *fDir = opendir(dirName.c_str());
    if (!fDir)
    {
        perror("opendir");
        return;
    }

    struct dirent *pDirent;
    while ((pDirent = readdir(fDir)) != nullptr)
    {
        string name = pDirent->d_name;

        if (name == "." || name == "..")
        {
            continue;
        }
        else if (pDirent->d_type == DT_REG) // 8=DT_REG 普通文件
        {
            string filePath = dirName + "/" + name;
            _filePathList.push_back(filePath);
        }
        else if (pDirent->d_type == DT_DIR) // 4=DT_DIR 目录
        {
            string strNextdir = dirName + "/" + name;
            traverse(strNextdir);
        }
    }

    closedir(fDir);
}

vector<string> &DirScanner::getFilePathList()
{
    return _filePathList;
}

} // namespace searchengine