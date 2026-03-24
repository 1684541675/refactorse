#include "Configuration.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

namespace searchengine {

using std::ifstream;
using std::stringstream;
using std::getline;
using std::cout;
using std::cerr;
using std::endl;

Configuration& Configuration::getInstance(const string &filepath)
{
    static Configuration instance(filepath);
    return instance;
}

unordered_map<string, string>& Configuration::getConfigMap()
{
    return _configMap;
}

Configuration::Configuration(const string &filepath)
: _filepath(filepath)
{
    loadConf();
}

void Configuration::loadConf()
{
    ifstream ifs(_filepath);

    if(!ifs) {
        cerr << "Configuration file open error: " << _filepath << endl;
        exit(1);
    }

    string line;
    string key, value;

    while(getline(ifs, line)) {
        stringstream ss(line);
        ss >> key >> value;
        _configMap.emplace(key, value);
    }

    cout << "[ Configuration loading... ]" << endl;

    for(const auto &item : _configMap) {
        cout << item.first << " " << item.second << endl;
    }

    cout << "[ Configuration load finished ]" << endl;
}

} // namespace searchengine