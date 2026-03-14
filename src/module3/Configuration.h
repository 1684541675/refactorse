#pragma once

#include <iostream>
#include <bits/stdc++.h>
using namespace std;

/*************************************************************
 *
 *  配置文件类
 *
 *************************************************************/
class Configuration
{
public:
    static Configuration *getInstance();
    unordered_map<string, string> &getConfigMap();

private:
    Configuration(const string &filepath = "../../conf/myconf.conf");
    ~Configuration(){};

    void loadConf();

    static void destroy();

private:
    string _filepath;
    unordered_map<string, string> _configMap; // <pages, ../yuliao/pages>
    static Configuration *_pInstance;
};
