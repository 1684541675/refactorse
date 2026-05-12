#include "Acceptor.h"

#include <sys/socket.h>
#include <cstdio>
#include <string>
using std::string;

#define CHECK_RET(ret, msg) \
    do { \
        if ((ret) == -1) { \
            perror(msg); \
            exit(EXIT_FAILURE); \
        } \
    } while (0)

namespace searchengine
{

Acceptor::Acceptor(const string &ip, unsigned short port)
:_listenSock()
,_serverAddr(ip, port)
{

}

void Acceptor::prepare()
{
    setReuseAddr(true);
    setReusePort(true);

    bind();

    listen();
}

void Acceptor::setReuseAddr(bool on)
{
    int opt = (int)on;
    int ret = setsockopt(_listenSock.fd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    CHECK_RET(ret, "setsockopt(reuse addr)");
}

void Acceptor::setReusePort(bool on)
{
    int opt = (int)on;
    int ret = setsockopt(_listenSock.fd(), SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
    CHECK_RET(ret, "setsockopt(reuse port)");
}

void Acceptor::bind()
{
    int ret = ::bind(_listenSock.fd(),
                     (struct sockaddr *)_serverAddr.getInetAddress(),
                     sizeof(struct sockaddr_in));
    CHECK_RET(ret, "bind");
}

void Acceptor::listen()
{
    int ret = ::listen(_listenSock.fd(), 256); // $ cat /proc/sys/net/ipv4/tcp_max_syn_backlog
    CHECK_RET(ret, "listen");
}

int Acceptor::accept()
{
    int connfd = ::accept(_listenSock.fd(), nullptr, nullptr);
    CHECK_RET(connfd, "accept");
    return connfd;
}

int Acceptor::fd()
{
    return _listenSock.fd();
}

}