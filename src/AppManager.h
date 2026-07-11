#pragma once

#include "App.h"

class AppManager
{
private:
    App* apps[20] = {nullptr};
    int appCount = 0;

public:
    void addApp(App* app);

    App* getApp(int index);

    int getCount();
};