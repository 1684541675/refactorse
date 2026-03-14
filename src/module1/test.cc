#include "Configuration.h"
#include "DictProducer.h"
#include "SplitTool.h"

int main()
{
    string cnYuliaoPath = Configuration::getInstance()->getConfigMap()["cnDir"];
    string enYuliaoPath = Configuration::getInstance()->getConfigMap()["enDir"];
    
    SplitTool tool;
    DictProducer cndict(cnYuliaoPath,&tool);
    DictProducer endict(enYuliaoPath);
    return 0;
}

