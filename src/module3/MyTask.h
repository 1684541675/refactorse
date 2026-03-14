#pragma once
#include "TcpConnection.h"
#include "WebPageSearcher.h"
#include "KeyRecommender.h"
#include "Dictionary.h"


#include <sw/redis++/redis++.h>
#include <unistd.h>
#include <iostream>
#include <bits/stdc++.h>
using namespace std;

using namespace sw::redis;
class MyTask
{
public:
    MyTask(const string &msg, const TcpConnectionPtr &connPtr, WebPageSearcher &webPageSearcher, KeyRecommender &recommender, sw::redis::Redis &redis)
    :_msg(msg)
    ,_connPtr(connPtr)
    ,_webPageSearcher(webPageSearcher)
    ,_recommender(recommender)
    ,_redis(redis)
    {

    } 

    void process(); // 由子线程（TheadPoll）调用！！！

private:
    string _msg;
    TcpConnectionPtr _connPtr;
    WebPageSearcher &_webPageSearcher;
    KeyRecommender &_recommender;
    sw::redis::Redis &_redis;
};
