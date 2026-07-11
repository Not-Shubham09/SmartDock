#include "AppManager.h"

void AppManager::addApp(App* app)
{
    if (appCount >= 20) return;
    apps[appCount++] = app;
}

App* AppManager::getApp(int index)
{
    if (index < 0 || index >= appCount || apps[index] == nullptr)
    {
        return apps[0]; // fallback to first app (ClockApp) if invalid
    }
    return apps[index];
}

int AppManager::getCount()
{
    return appCount;
}