#include "Socket.h"
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#include <bits/stdc++.h>
using namespace std;

Socket::Socket()
{
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if(_fd<0)
    {
        perror("socket");
        return;
    }
}

Socket::Socket(int fd)
:_fd(fd)
{

}

Socket::~Socket()
{
    while (close(_fd) < 0 && errno == EINTR);
}

int Socket::fd() const
{
    return _fd;
}

void Socket::shutDownWrite()
{
    int ret = shutdown(_fd, SHUT_WR); // <sys/socket.h>
    if(ret<0)
    {
        perror("shutdown");
        return;
    }
}

void Socket::setNonBlock()
{
    int flags = fcntl(_fd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(_fd, F_SETFL, flags);
}

