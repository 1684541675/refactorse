#include "MyLog.h"

namespace searchengine
{

MyLog::MyLog()
:_root(Category::getRoot().getInstance("SearchEngine"))
{
    auto ptnLayout = new PatternLayout();
    ptnLayout->setConversionPattern("%d %c [%p] %m%n");

    auto pFileapp = new FileAppender("fileapp", "../../log/SearchEngine.log");
    pFileapp->setLayout(ptnLayout);

    _root.setPriority(Priority::DEBUG);
    _root.addAppender(pFileapp);
}

MyLog &MyLog::getInstance()
{
    static MyLog instance;
    return instance;
}


MyLog::~MyLog()
{
    Category::shutdown();
}

}