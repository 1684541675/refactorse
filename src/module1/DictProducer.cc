#include "DictProducer.h"
#include "SplitTool.h"
#include "Configuration.h"
#include <dirent.h>


DictProducer::DictProducer(const string &dir)
:_endir(dir)
{
    string enDictPath = Configuration::getInstance()->getConfigMap()["enDict"];
    string enDictIndex = Configuration::getInstance()->getConfigMap()["enDictIndex"];
    string enStopDictPath = Configuration::getInstance()->getConfigMap()["enStop"];

    loadStopWord(enStopDictPath);

    buildEnDict();
    storeDict(enDictPath.c_str());
    buildIndex();
    storeIndex(enDictIndex.c_str());
    cout << "Build En Dict and DictIndex OK" << endl;
} //英文

DictProducer::DictProducer(const string &dir, SplitTool *splitTool)
:_dir(dir)
,_splitTool(splitTool)
{
    string dictPath = Configuration::getInstance()->getConfigMap()["dict"];
    string dictIndex = Configuration::getInstance()->getConfigMap()["dictIndex"];
    string cnStopDictPath = Configuration::getInstance()->getConfigMap()["cnStop"];

    loadStopWord(cnStopDictPath);

    buildCnDict();
    storeDict(dictPath.c_str());
    buildIndex();
    storeIndex(dictIndex.c_str());
    cout << "Build Cn Dict and DictIndex OK" << endl;
} //中文

void DictProducer::loadStopWord(string stopDictPath)
{
    _stopWord.clear();
    ifstream ifs(stopDictPath);
    if (!ifs.good())
    {
        cout << "ifstream open error" << endl;
        return;
    }
    string line, word;
    while (getline(ifs, line))
    {
        istringstream iss(line); //处理每一行
        while (iss >> word)
        {
            _stopWord.insert(word);
        }
    }
    ifs.close();
}

void DictProducer::buildEnDict()
{
    stringstream ss;
    string line, word, in_word;

    _files.clear();
    getFiles(_endir);

    for (auto &file : _files)
    {
        ifstream ifs(file);
        if (!ifs.good())
        {
            cout << "ifstream open error" << endl;
            return;
        }

        while (getline(ifs, line))
        {
            in_word.clear();
            istringstream iss(line); //处理每一行

            while (iss >> word)
            {
                // cout << word << endl;
                in_word.clear();
                for (char c : word)
                {
                    //去大写
                    if (c >= 'A' && c <= 'Z')
                    {
                        c += 32;
                    }
                    //去符号
                    if (isalpha(c))
                    {
                        in_word += c;
                    }
                    else
                    {
                        break;
                    }
                }

                if (in_word[in_word.size() - 1] == '-')
                {
                    in_word.resize(in_word.size() - 1);
                }

                if (in_word != " " && find(_stopWord.begin(), _stopWord.end(), in_word) == _stopWord.end())
                {
                    ++_dict[in_word];
                }
                else
                {
                    continue;
                }
            }
        }
        ifs.close();
    }
}

void DictProducer::buildCnDict()
{
    getFiles(_dir);
    for (auto &file : _files)
    {
        ifstream ifs(file, std::ios::ate);
        if (!ifs.good())
        {
            cout << "openArtDict file fail" << endl;
            return;
        }

        size_t length = ifs.tellg(); //整片文章长度
        ifs.seekg(std::ios_base::beg);
        char *buff = new char[length + 1]; //一次性读入
        ifs.read(buff, length + 1);
        string txt(buff);

        delete []buff;

        vector<string> tmp = _splitTool->cut(txt);

        for (auto &elem : tmp)
        {
            if (!_stopWord.count(elem) && getByteNum_UTF8(elem[0]) == 3)
            {
                ++_dict[elem];
            }
        }
        ifs.close();
    }
}

void DictProducer::buildIndex()
{
    int i = 0;
    for (auto &elem : _dict)
    {
        string word = elem.first;
        size_t charNums = word.size() / getByteNum_UTF8(word[0]); //获取字符长度
        for (size_t idx = 0, n = 0; n != charNums; ++idx, ++n)
        {
            size_t charLen = getByteNum_UTF8(word[idx]);
            string subWord = word.substr(idx, charLen);
            _index[subWord].insert(i);
            idx += (charLen - 1);
        }
        ++i;
    }
}
void DictProducer::storeDict(const char *filepath)
{
    ofstream ofs(filepath);
    if (!ofs.good())
    {
        cout << "error in storeDict" << endl;
        return;
    }
    for (auto &it : _dict)
    {
        ofs << it.first << "  " << it.second << endl;
    }
    ofs.close();
}

void DictProducer::storeIndex(const char *filepath)
{
    ofstream ofs(filepath);
    if (!ofs.good())
    {
        cout << "error in store index" << endl;
        return;
    }
    for (auto &it : _index)
    {
        ofs << it.first << "  ";
        for (auto &set_it : it.second)
        {
            ofs << set_it << " ";
        }
        ofs << endl;
    }
    ofs.close();
}

void DictProducer::getFiles(string dir) //获取文件绝对路径
{

    DIR *dirp = opendir(dir.c_str());
    if(!dirp)
    {
        perror("opendir");
        return;
    }

    struct dirent *pdirent;
    //遍历指定目录下的所有文件和子目录，并将它们的路径存储在一个容器中，同时跳过 . 和 .. 这两个特殊目录。
    while ((pdirent = readdir(dirp)) != NULL)
    {
        if (strcmp(pdirent->d_name, ".") == 0 || strcmp(pdirent->d_name, "..") == 0)
        {
            continue;
        }

        string filePath;
        filePath = dir + "/" + pdirent->d_name;

        _files.push_back(filePath);
    }
    closedir(dirp);
}

size_t DictProducer::getByteNum_UTF8(const char byte)
{
    int byteNum = 0;
    for (size_t i = 0; i < 6; ++i)
    {
        if (byte & (1 << (7 - i)))
        {
            ++byteNum;
        }
        else
        {
            break;
        }
    }
    return byteNum == 0 ? 1 : byteNum;
}
