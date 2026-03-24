#include "OffsetProcesser.h"
#include "WebPage.h"

#include <vector>
#include <utility>
#include <cstddef>  // size_t

namespace searchengine
{

using std::vector;
using std::pair;

OffsetProcesser::OffsetProcesser(vector<WebPage> &pagelist,
                                 vector<pair<size_t, size_t>> &offsetlib)
: _pagelist(pagelist)
, _offsetlib(offsetlib)
{
    
}

/**
 * 生成偏移库 _offsetlib
 */
void OffsetProcesser::process()
{
    size_t offset = 0;
    for (auto &page : _pagelist)
    {
        size_t textlength = page.getDoc().size();
        _offsetlib.push_back({offset, textlength});
        offset += textlength;
    }
}

} // namespace searchengine