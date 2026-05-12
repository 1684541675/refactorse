#include "SocketIO.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>
#include <cstdio>

namespace searchengine
{

SocketIO::SocketIO(int fd)
: _fd(fd)
{

}

/*
 *  1. 从 RCV 中至少读取 count 个字节到 buf 中
 *  2. 返回成功读到的字节数
 *  3. 出错返回 count + 1
 *  4. 不保证字符串 buf 以 \0 结尾
 */
size_t SocketIO::readn(void *buf, size_t count)
{
    size_t left = count;                     // 还需要读取的字节数
    char *ptr = static_cast<char *>(buf);    // 当前写入位置

    while (left > 0)
    {
        ssize_t ret = 0;

        do
        {
            ret = recv(_fd, ptr, left, 0);
        } while (ret == -1 && errno == EINTR);

        if (ret == -1)
        {
            perror("recv");
            return count + 1;
        }
        else if (ret == 0)
        {
            fprintf(stderr, "recv: peerFd(%d) disconnected\n", _fd);
            break;
        }
        else
        {
            ptr += ret;
            left -= ret;
        }
    }

    return count - left;
}

/*
 *  1. 将 buf 中的恰好 count 个字节写入到 SND 中
 *  2. 返回成功写入的字节数
 *  3. 出错返回 count + 1
 *  4. 未写完返回值将 < count
 */
size_t SocketIO::writen(const void *buf, size_t count)
{
    size_t left = count;
    const char *ptr = static_cast<const char *>(buf);

    while (left > 0)
    {
        ssize_t ret = 0;

        do
        {
            ret = send(_fd, ptr, left, 0);
        } while (ret == -1 && errno == EINTR);

        if (ret == -1)
        {
            perror("send");
            return count + 1;
        }
        else if (ret == 0)
        {
            fprintf(stderr, "send: peerFd(%d) disconnected\n", _fd);
            break;
        }
        else
        {
            ptr += ret;
            left -= ret;
        }
    }

    return count - left;
}

/*
 *  1. 从 RCV 中读取一行数据到 buf 中，这一行最大不超过 max 个字节
 *  2. 若成功读入一整行则返回成功读到的字节数
 *     若读完 max 仍未找到 \n，返回成功读到的字节数
 *     若连接已断开，返回一个小于 strlen(buf) 的值
 *  3. 出错返回 max + 1
 *  4. 不保证字符串 buf 以 \0 结尾
 */
size_t SocketIO::readline(char *buf, size_t max)
{
    if (max == 0)
    {
        return 0;
    }

    size_t left = max - 1;   // 预留一个位置给 '\0'
    size_t total = 0;        // 已经真正读取的字节数
    char *ptr = buf;         // 下一个写入位置

    while (left > 0)
    {
        ssize_t ret = 0;

        do
        {
            ret = recv(_fd, ptr, left, MSG_PEEK);
        } while (ret == -1 && errno == EINTR);

        if (ret == -1)
        {
            perror("recv");
            return max + 1;
        }
        else if (ret == 0)
        {
            fprintf(stderr, "recv: peerFd(%d) disconnected\n", _fd);
            break;
        }

        size_t nread = 0;
        bool foundNewline = false;

        for (size_t i = 0; i < static_cast<size_t>(ret); ++i)
        {
            ++nread;

            if (ptr[i] == '\n')
            {
                foundNewline = true;
                break;
            }
        }

        size_t real = readn(ptr, nread);

        if (real > nread)
        {
            return max + 1;
        }

        ptr += real;
        total += real;
        left -= real;

        if (real < nread)
        {
            buf[total] = '\0';
            return max + 1;
        }

        if (foundNewline)
        {
            break;
        }
    }

    buf[total] = '\0';
    return total;
}


} // namespace searchengine
