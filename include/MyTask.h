#pragma once
#include "TcpConnection.h"
#include "WebPageSearcher.h"
#include "KeyRecommender.h"

#include <sw/redis++/redis++.h>

#include <string>
using std::string;


namespace searchengine
{

class MyTask
{
public:
    MyTask(const string &msg, const TcpConnectionPtr &connPtr, 
        WebPageSearcher &webPageSearcher, KeyRecommender &recommender, 
        sw::redis::Redis &redis);

    void process(); // 由子线程（TheadPoll）调用！！！

private:
    string _msg;
    TcpConnectionPtr _connPtr;
    WebPageSearcher &_webPageSearcher;
    KeyRecommender &_recommender;
    sw::redis::Redis &_redis;
};

}