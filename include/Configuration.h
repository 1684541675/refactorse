#pragma once

#include <string>
#include <unordered_map>


namespace searchengine {

using std::string;
using std::unordered_map;

class Configuration {
public:
    static Configuration& getInstance(const string &filepath = "../../conf/myconf.conf");

    unordered_map<string, string>& getConfigMap();

private:
    Configuration(const string &filepath);
    ~Configuration() = default;

    Configuration(const Configuration&) = delete;
    Configuration& operator=(const Configuration&) = delete;

    void loadConf();

private:
    string _filepath;
    unordered_map<string, string> _configMap;
};

} // namespace searchengine