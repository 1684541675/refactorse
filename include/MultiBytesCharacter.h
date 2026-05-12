#pragma once
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace searchengine
{

inline size_t nBytesCode(const char ch)
{
    int byteNum = 0;

    for (size_t i = 0; i < 6; ++i)
    {
        if (ch & (1 << (7 - i)))
        {
            ++byteNum;
        }
        else
        {
            break;
        }
    }

    return byteNum == 0 ? 1 : byteNum;
}

/**
 *  获取从 str[pos] 开始往后 N 个字符所占的字节数
 *
 *  1. pos 表示起始字节下标
 *  2. limit 是最多允许返回的字节数
 *  3. 如果超过 limit，则直接返回 limit
 */
inline size_t howManyBytesWithNCharacter(const string &str, size_t pos, size_t limit, size_t N)
{
    size_t totalBytes = 0;

    for (size_t i = 0; i < N && pos < str.size(); ++i)
    {
        size_t nBytes = nBytesCode(str[pos]);

        if (totalBytes + nBytes > limit)
        {
            return limit;
        }

        totalBytes += nBytes;
        pos += nBytes;
    }

    return totalBytes;
}

/**
 *  获取 str 前 end 个字节中，所有字符的起始字节下标
 *
 *  1. end 表示只扫描 str[0, end) 这一段
 *  2. 返回值中保存每个字符的起始字节位置
 */
inline vector<size_t> getPosPerCharactor(const string &str, size_t end)
{
    vector<size_t> res;

    end = std::min(end, str.size());

    for (size_t pos = 0; pos < end; )
    {
        res.push_back(pos);

        size_t nBytes = nBytesCode(str[pos]);
        pos += nBytes;
    }

    return res;
}

}