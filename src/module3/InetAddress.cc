#include "InetAddress.h"
#include <arpa/inet.h>
#include <cstring>

namespace searchegine
{

InetAddress::InetAddress(const string &ip, unsigned short port)
{
    memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = inet_addr(ip.c_str());
}

InetAddress::InetAddress(const struct sockaddr_in &addr)
:_addr(addr)
{

}

string InetAddress::ip() const
{
    return string(inet_ntoa(_addr.sin_addr)); // 大端存储的无符号整数 -> 点分十进制字符串
}

unsigned short InetAddress::port() const
{
    return ntohs(_addr.sin_port); // net to host by short
}

const struct sockaddr_in *InetAddress::getInetAddress() const
{
    return &_addr;
}

}