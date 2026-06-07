#include "AppManager.h"

void AppManager::addApp(App* app)
{
    apps[appCount++] = app;
}

App* AppManager::getApp(int index)
{
    return apps[index];
}

int AppManager::getCount()
{
    return appCount;
}