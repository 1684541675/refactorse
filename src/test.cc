#include <sw/redis++/redis++.h>
#include <iostream>
#include <exception>
#include <string>

using std::cout;
using std::endl;
using std::string;

int main()
{
    try
    {
        // 默认连接 127.0.0.1:6379
        sw::redis::Redis redis("tcp://127.0.0.1:6379");

        // 测试 ping
        auto pong = redis.ping();
        cout << "PING result: " << pong << endl;

        // 写入 key
        redis.set("test_key", "hello redis");

        // 读取 key
        auto val = redis.get("test_key");
        if (val)
        {
            cout << "test_key = " << *val << endl;
        }
        else
        {
            cout << "test_key not found" << endl;
        }

        // 删除 key
        redis.del("test_key");

        cout << "Redis test success!" << endl;
    }
    catch (const sw::redis::Error &e)
    {
        std::cerr << "Redis error: " << e.what() << endl;
        return 1;
    }
    catch (const std::exception &e)
    {
        std::cerr << "std exception: " << e.what() << endl;
        return 1;
    }

    return 0;
}
