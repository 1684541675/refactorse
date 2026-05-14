# SearchEngine

## 项目简介
基于 Linux C++ 实现的小型搜索引擎服务端系统，支持关键词推荐与网页检索。
项目分为离线构建和在线服务两部分：离线阶段完成网页解析、去重、分词、词典与倒排索引构建；在线阶段基于 Reactor + 线程池处理客户端请求，并结合 LRUCache 与 Redis 优化热点查询。

## 技术栈
C++、Linux、Socket、epoll、Reactor、线程池、CppJieba、Simhash、TF-IDF、Redis、LRUCache、tinyxml2、nlohmann/json

## 项目功能
- 支持关键词推荐
- 支持网页检索
- 支持中文分词与停用词过滤
- 支持网页去重
- 支持倒排索引构建
- 支持多客户端并发访问
- 支持本地 LRUCache 与 Redis 缓存

## 系统架构
1. 离线模块
   - RSS/XML 网页解析
   - 网页去重
   - 中文分词
   - 词典构建
   - 倒排索引构建

2. 在线模块
   - TcpServer
   - EventLoop
   - TcpConnection
   - ThreadPool
   - KeyRecommender
   - WebPageSearcher
   - CacheManager

## 核心亮点
- 使用 epoll 实现 IO 多路复用
- 基于 Reactor 模型封装网络服务端
- 使用线程池将网络 IO 与业务计算解耦
- 通过 eventfd/counter 机制实现工作线程通知 IO 线程发送响应
- 使用 Simhash 对网页进行去重
- 使用 TF-IDF 和余弦相似度完成网页相关度排序
- 使用 LRUCache + Redis 提升热点查询性能
