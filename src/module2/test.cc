#include "PageLib.h"
#include "Configuration.h"
int main()
{
    PageLib lib(Configuration::getInstance()->getConfigMap()["pages"]);
    lib.create();
    lib.store();
    return 0;
}
