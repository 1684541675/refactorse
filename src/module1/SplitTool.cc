#include "SplitTool.h"
#include "Configuration.h"

namespace searchengine {

using std::string;
using std::vector;

SplitTool::SplitTool()
: _jieba(
    Configuration::getInstance().getConfigMap()["jieba_dict"],
    Configuration::getInstance().getConfigMap()["jieba_hmm"],
    Configuration::getInstance().getConfigMap()["jieba_user_dict"]
)
{

}

vector<string> SplitTool::cut(const string &sentence)
{
    vector<string> result;
    _jieba.CutForSearch(sentence, result);
    return result;
}

}