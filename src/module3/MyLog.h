#pragma once
#include <log4cpp/Category.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/Priority.hh>
#include <iostream>
#include <bits/stdc++.h>
using namespace log4cpp;
using namespace std;
#define addPrefix(msg) string().append("[").append(__FILE__).append(":").append(__FUNCTION__).append(".").append(to_string(__LINE__)).append("]").append(msg).c_str()

#define prefix(msg) (string(__FILE__)+string(": ")+\
                string(__FUNCTION__)+string(": ")+\
                string(to_string(__LINE__))+string(": ")+msg).c_str()
#define LogWarn(msg, ...) MyLog::getInstance()->warn(addPrefix(msg), ##__VA_ARGS__)
#define LogError(msg, ...) MyLog::getInstance()->error(prefix(msg), ##__VA_ARGS__)
#define LogInfo(msg, ...) MyLog::getInstance()->info(addPrefix(msg), ##__VA_ARGS__)
#define LogDebug(msg, ...) MyLog::getInstance()->debug(addPrefix(msg), ##__VA_ARGS__)

class MyLog
{
public:
    static MyLog *getInstance();
    static void destroy();

    template <typename... T>
    void warn(const char *msg, T... args);

    template <typename... T>
    void error(const char *msg, T... args);

    template <typename... T>
    void info(const char *msg, T... args);

    template <typename... T>
    void debug(const char *msg, T... args);

private:
    MyLog();
    ~MyLog();

private:
    static MyLog *_pInstance;
    Category &_root;
};

template <typename... T>
void MyLog::warn(const char *msg, T... args)
{
    _root.warn(msg, args...);
}

template <typename... T>
void MyLog::error(const char *msg, T... args)
{
    _root.error(msg, args...);
}

template <typename... T>
void MyLog::info(const char *msg, T... args)
{
    _root.info(msg, args...);
}
