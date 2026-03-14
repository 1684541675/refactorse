#include "EchoServer.h"
#include "MyTask.h"
#include "MyLog.h"
#include "TimerTask.h"
#include "Configuration.h"

EchoServer::EchoServer(const string &ip, unsigned short port)
:_pool(INIT_WORKER_NUM, INIT_TASKQUEUE_CAPACITY)
,_server(ip, port)
,_webPageSearcher()
,_recommender()
,_redis("tcp://127.0.0.1:6379")
,_timerThread(bind(&TimerTask::process, TimerTask()),
                   stoi(Configuration::getInstance()->getConfigMap()["initTime"]),
                   stoi(Configuration::getInstance()->getConfigMap()["periodicTime"]))
{

}

void EchoServer::start()
{
    _pool.start();

    _timerThread.start();

    using namespace std::placeholders;
    _server.setConnectionCallBack(bind(&EchoServer::onConnection, this, _1));
    _server.setMessageCallBack(bind(&EchoServer::onMessage, this, _1));
    _server.setCloseCallBack(bind(&EchoServer::onClose, this, _1));

    _server.start();
}

void EchoServer::stop()
{
    _server.stop();

    _timerThread.stop();

    _pool.stop();
}

void EchoServer::onConnection(const TcpConnectionPtr &connPtr)
{
    LogInfo("\n\t%s connected", connPtr->show().c_str());
    cout << connPtr->show() << " connected!" << endl;
}

void EchoServer::onMessage(const TcpConnectionPtr &connPtr)
{
    // recv
    string msg = connPtr->recv();

    // decode -> compute -> encode -> send
    MyTask task(msg, connPtr, _webPageSearcher, _recommender, _redis);
    _pool.addTask(bind(&MyTask::process, task)); // 因此 ThreadPool ..> MyTask
}

void EchoServer::onClose(const TcpConnectionPtr &connPtr)
{
    LogInfo("\n\t%s disconnected", connPtr->show().c_str());
    cout << connPtr->show() << " disconnected!" << endl;
}

