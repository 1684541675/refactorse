#include <iostream>
#include <bits/stdc++.h>
using namespace std;
#include "Acceptor.h"

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
    if(ret<0)
    {
        perror("setsockopt(reuse addr)");
    }
}

void Acceptor::setReusePort(bool on)
{
    int opt = (int)on;
    int ret = setsockopt(_listenSock.fd(), SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
    ERROR_CHECK(ret, -1, "setsockopt(reuse port)");
}

void Acceptor::bind()
{
    int ret = ::bind(_listenSock.fd(),
                     (struct sockaddr *)_serverAddr.getInetAddress(),
                     sizeof(struct sockaddr));
    ERROR_CHECK(ret, -1, "bind");
}

void Acceptor::listen()
{
    int ret = ::listen(_listenSock.fd(), 256); // $ cat /proc/sys/net/ipv4/tcp_max_syn_backlog
    ERROR_CHECK(ret, -1, "listen");
}

int Acceptor::accept()
{
    int peerfd = ::accept(_listenSock.fd(), nullptr, nullptr);
    if(peerfd<0)
    {
        perror("accept");
        return -1;
    }
    return peerfd;
}

int Acceptor::fd()
{
    return _listenSock.fd();
}

