#include "Dictionary.h"
#include "Configuration.h"

Dictionary *Dictionary::_singletonDict = getInstance(); //饿汉模式
Dictionary *Dictionary::getInstance()
{
    if (_singletonDict == nullptr)
    {
        _singletonDict = new Dictionary();
        _singletonDict->initDict();
        _singletonDict->initIndex();
        atexit(destory);
    }
    return _singletonDict;
}

void Dictionary::destory()
{
    if (_singletonDict)
    {
        delete _singletonDict;
        _singletonDict = nullptr;
    }
}

void Dictionary::initDict()
{
    cout << "initialize dictionary" << endl;
    string dictPath = Configuration::getInstance()->getConfigMap()["dict"];
    //string enDictPath = Configuration::getInstance()->getConfigMap()["enDict"];

    ifstream ifs(dictPath);
    //ifstream ifs(enDictPath);
    if (!ifs.good())
    {
        cout << "ifstream open file" << string(dictPath) << " error !" << endl;
        return;
    }
    string line;
    while (getline(ifs, line))
    {
        istringstream iss(line);
        string word, freq;
        iss >> word >> freq;
        _dict.push_back(pair<string, int>(word, stoi(freq)));
    }
}

void Dictionary::initIndex() // key : set<id>
{
    cout << "initialize index" << endl;
    string dictIndexPath = Configuration::getInstance()->getConfigMap()["dictIndex"];
    //string enDictIndexPath = Configuration::getInstance()->getConfigMap()["enDictIndex"];

    ifstream ifs(dictIndexPath);
    //ifstream ifs(enDictIndexPath);
    if (!ifs.good())
    {
        cout << "ifstream open file" << string(dictIndexPath) << " error !" << endl;
        return;
    }
    string line;
    while (getline(ifs, line))
    {
        istringstream iss(line);
        string word, locate;
        iss >> word;
        while (iss >> locate)
        {
            _index[word].insert(stoi(locate));
        }
    }
}

const vector<pair<string, int>> &Dictionary::getDict()
{
    return _dict;
}

const map<string, set<int>> &Dictionary::getIndexTable()
{
    return _index;
}


