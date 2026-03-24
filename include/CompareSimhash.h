#pragma once

#include "../3rdparty/simhash-cppjieba/Simhasher.hpp"

#include <vector>
#include <unordered_map>
#include <cstdint>

namespace searchengine
{

using std::vector;
using std::unordered_map;

class WebPage;

/*************************************************************
 *
 *  网页比较类（去重）
 *
 *************************************************************/
class CompareSimhash
{
public:
    CompareSimhash();

    bool cut(const WebPage &);

private:
    unordered_map<uint64_t, vector<uint64_t>> _bucketA;
    unordered_map<uint64_t, vector<uint64_t>> _bucketB;
    unordered_map<uint64_t, vector<uint64_t>> _bucketC;
    unordered_map<uint64_t, vector<uint64_t>> _bucketD;
    const size_t topN = 5;

    simhash::Simhasher _simhasher; 
};

} // namespace searchengine