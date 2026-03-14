#include "Configuration.h"
#include "DictProducer.h"
#include "SplitTool.h"

#include <string>

using namespace searchengine;
using std::string;

int main()
{
    auto &config = Configuration::getInstance().getConfigMap();

    string cnYuliaoPath = config["cnDir"];
    string enYuliaoPath = config["enDir"];

    SplitTool tool;

    DictProducer cnDict(cnYuliaoPath, &tool);
    DictProducer enDict(enYuliaoPath);

    return 0;
}