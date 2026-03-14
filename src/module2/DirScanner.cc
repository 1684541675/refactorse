#include "DirScanner.h"
#include <dirent.h>

DirScanner::DirScanner(const string &dirPath)
:_dirPath(dirPath)
{
    cout << "DirScanner()\n";
    traverse();
    for (auto &item : _filePathList)
    {
        cout << item << "\n";
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
    if(!fDir)
    {
        perror("opendir");
        return ;
    }

    struct dirent *pDirent;
    while (NULL != (pDirent = readdir(fDir)))
    {
        if (strcmp(pDirent->d_name, ".") == 0 || strcmp(pDirent->d_name, "..") == 0)
        {
            continue;
        }
        else if (pDirent->d_type == 8)//8=DT_REG 表示普通文档
        {
            string filePath;
            filePath = dirName + "/" + pDirent->d_name;
            _filePathList.push_back(filePath);
        }
        else if (pDirent->d_type == 4)//4=DR_DIR 表示目录
        {
            string strNextdir = dirName + "/" + pDirent->d_name;
            traverse(strNextdir);
        }
    }

    closedir(fDir);
}

vector<string> &DirScanner::getFilePathList()
{
    return _filePathList;
}

