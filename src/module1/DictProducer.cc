#include "DictProducer.h"
#include "SplitTool.h"
#include "Configuration.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <dirent.h>


namespace searchengine {

using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::stringstream;
using std::getline;
using std::cout;
using std::endl;

DictProducer::DictProducer(const string &dir)
: _endir(dir)
, _splitTool(nullptr)
{
    auto &config = Configuration::getInstance().getConfigMap();

    string enDictPath = config["enDict"];
    string enDictIndex = config["enDictIndex"];
    string enStopDictPath = config["enStop"];

    loadStopWord(enStopDictPath);

    buildEnDict();
    storeDict(enDictPath.c_str());
    buildIndex();
    storeIndex(enDictIndex.c_str());

    cout << "Build En Dict and DictIndex OK" << endl;
}

// 中文
DictProducer::DictProducer(const string &dir, SplitTool *splitTool)
: _dir(dir)
, _splitTool(splitTool)
{
    auto &config = Configuration::getInstance().getConfigMap();

    string dictPath = config["dict"];
    string dictIndex = config["dictIndex"];
    string cnStopDictPath = config["cnStop"];

    loadStopWord(cnStopDictPath);

    buildCnDict();
    storeDict(dictPath.c_str());
    buildIndex();
    storeIndex(dictIndex.c_str());

    cout << "Build Cn Dict and DictIndex OK" << endl;
}

void DictProducer::loadStopWord(const string &stopDictPath)
{
    _stopWord.clear();

    ifstream ifs(stopDictPath);
    if(!ifs) {
        cout << "stopword file open error" << endl;
        return;
    }

    string line, word;

    while(getline(ifs, line)) {
        istringstream iss(line);
        while(iss >> word) {
            _stopWord.insert(word);
        }
    }
}

void DictProducer::buildEnDict()
{
    string line, word;

    _files.clear();
    getFiles(_endir);

    for(auto &file : _files) {

        ifstream ifs(file);
        if(!ifs) {
            cout << "file open error" << endl;
            continue;
        }

        while(getline(ifs, line)) {

            istringstream iss(line);

            while(iss >> word) {

                string cleanWord;

                for(char c : word) {

                    c = tolower(c);

                    if(isalpha(c)){
                        cleanWord += c;
                    }  
                    else{
                         break;
                    }      
                }

                if(cleanWord.empty()){
                    continue;
                }
                    
                if(!_stopWord.count(cleanWord)){
                    ++_dict[cleanWord];
                }
                    
            }
        }
    }
}

void DictProducer::buildCnDict()
{
    getFiles(_dir);

    for(auto &file : _files) {

        ifstream ifs(file);
        if(!ifs) {
            cout << "file open error" << endl;
            continue;
        }

        stringstream buffer;
        buffer << ifs.rdbuf();

        string text = buffer.str();

        auto words = _splitTool->cut(text);

        for(auto &word : words) {

            if(!word.empty() &&!_stopWord.count(word) && getByteNum_UTF8(word[0]) == 3){
                ++_dict[word];
            }
                
        }
    }
}

void DictProducer::buildIndex()
{
    int i = 0;

    for(auto &elem : _dict) {

        const string &word = elem.first;

        size_t charNums = word.size() / getByteNum_UTF8(word[0]);

        for(size_t idx = 0, n = 0; n != charNums; ++idx, ++n) {

            size_t charLen = getByteNum_UTF8(word[idx]);

            string subWord = word.substr(idx, charLen);

            _index[subWord].insert(i);

            idx += charLen - 1;
        }

        ++i;
    }
}

void DictProducer::storeDict(const char *filepath)
{
    ofstream ofs(filepath);

    if(!ofs) {
        cout << "storeDict open error" << endl;
        return;
    }

    for(auto &it : _dict) {
        ofs << it.first << " " << it.second << endl;
    }
}

void DictProducer::storeIndex(const char *filepath)
{
    ofstream ofs(filepath);

    if(!ofs) {
        cout << "storeIndex open error" << endl;
        return;
    }

    for(auto &it : _index) {

        ofs << it.first << " ";

        for(auto &idx : it.second){
            ofs << idx << " ";
        }

        ofs << endl;
    }
}

void DictProducer::getFiles(const string &dir)
{
    DIR *dirp = opendir(dir.c_str());

    if(!dirp) {
        perror("opendir");
        return;
    }

    struct dirent *pdirent;

    while((pdirent = readdir(dirp)) != nullptr) {

        string name = pdirent->d_name;

        if(name == "." || name == "..")
        {
            continue;
        }

        string filePath = dir + "/" + name;

        _files.push_back(filePath);
    }

    closedir(dirp);
}

size_t DictProducer::getByteNum_UTF8(char byte)
{
    int byteNum = 0;

    for(size_t i = 0; i < 6; ++i) {

        if(byte & (1 << (7-i))){
            ++byteNum;
        }       
        else{
             break;
        }     
    }

    return byteNum == 0 ? 1 : byteNum;
}

}

