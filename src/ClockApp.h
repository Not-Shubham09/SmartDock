#pragma once

#include "App.h"

class ClockApp : public App
{
private:
    unsigned long lastUpdate = 0;
    char lastTimeStr[10] = {0};
    char lastDateStr[32] = {0};
    
    enum SyncState {
        STATE_DISCONNECTED,
        STATE_SYNCING,
        STATE_SYNCED
    };
    SyncState lastState = STATE_DISCONNECTED;

public:
    const char* getName() override;

    void draw(lgfx::LGFX_Device* display) override;

    void update(lgfx::LGFX_Device* display) override;
};