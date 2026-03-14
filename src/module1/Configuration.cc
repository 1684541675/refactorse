#include "Configuration.h"

Configuration *Configuration::_pInstance = Configuration::getInstance(); // 饿汉模式

Configuration::Configuration(const string &filepath)
:_filepath(filepath)
{
    loadConf();
}

/**
 *  加载配置文件，并将配置信息存入 _configMap
 */
void Configuration::loadConf()
{
    ifstream ifs(_filepath);

    string lines;
    string first, second;
    while (getline(ifs, lines))
    {
        stringstream ss(lines);
        ss >> first >> second;
        _configMap[first] = second;
    }

    cout << "[ _configMag is loading... ]\n"
              << endl;
    for (auto &item : _configMap)
    {
        cout << item.first << " " << item.second << endl;
    }
    cout << "\n[ _configMag finish loading ]\n"
              << endl;

    ifs.close();
}

Configuration *Configuration::getInstance()
{
    if (_pInstance == nullptr)
    {
        _pInstance = new Configuration();
        atexit(destroy);
    }

    return _pInstance;
}

void Configuration::destroy()
{
    cout << "void Configuration::destroy()" << endl;

    if (_pInstance)
    {
        delete _pInstance;
        _pInstance = nullptr;
    }
}

unordered_map<string, string> &Configuration::getConfigMap()
{
    return _configMap;
}

