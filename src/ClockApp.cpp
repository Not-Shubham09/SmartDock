#include "ClockApp.h"
#include "WiFiManager.h"
#include <time.h>

extern WiFiManager wifiManager;

const char* ClockApp::getName()
{
    return "Clock";
}

void ClockApp::draw(lgfx::LGFX_Device* display)
{
    display->fillScreen(TFT_BLACK);
    display->drawRoundRect(10, 10, 220, 300, TFT_CYAN);

    display->setTextColor(TFT_CYAN);
    display->setTextSize(3);
    display->drawCentreString("CLOCK", 120, 30, 2);

    display->setTextColor(TFT_WHITE);
    display->setTextSize(2);
    display->drawCentreString("SmartDock OS", 120, 250, 2);

    time_t now = time(nullptr);
    struct tm timeInfo;
    localtime_r(&now, &timeInfo);

    if (!wifiManager.isConnected())
    {
        display->drawCentreString("Connecting WiFi...", 120, 140, 2);
        lastState = STATE_DISCONNECTED;
    }
    else if (timeInfo.tm_year < 120)
    {
        display->drawCentreString("Syncing Time...", 120, 140, 2);
        lastState = STATE_SYNCING;
    }
    else
    {
        lastState = STATE_SYNCED;

        strftime(lastTimeStr, sizeof(lastTimeStr), "%H:%M:%S", &timeInfo);
        strftime(lastDateStr, sizeof(lastDateStr), "%b %d, %Y", &timeInfo);

        display->setTextSize(3);
        display->drawCentreString(lastTimeStr, 120, 120, 2);

        display->setTextSize(2);
        display->drawCentreString(lastDateStr, 120, 180, 2);
    }
}

void ClockApp::update(lgfx::LGFX_Device* display)
{
    unsigned long nowMs = millis();
    if (nowMs - lastUpdate < 1000)
    {
        return;
    }
    lastUpdate = nowMs;

    SyncState currentState = STATE_DISCONNECTED;
    time_t now = time(nullptr);
    struct tm timeInfo;
    localtime_r(&now, &timeInfo);

    if (!wifiManager.isConnected())
    {
        currentState = STATE_DISCONNECTED;
    }
    else if (timeInfo.tm_year < 120)
    {
        currentState = STATE_SYNCING;
    }
    else
    {
        currentState = STATE_SYNCED;
    }

    if (currentState != lastState)
    {
        draw(display);
        return;
    }

    if (currentState == STATE_SYNCED)
    {
        char timeStr[10];
        char dateStr[32];
        strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &timeInfo);
        strftime(dateStr, sizeof(dateStr), "%b %d, %Y", &timeInfo);

        if (strcmp(timeStr, lastTimeStr) != 0)
        {
            display->fillRect(11, 120, 218, 48, TFT_BLACK);
            display->setTextColor(TFT_WHITE);
            display->setTextSize(3);
            display->drawCentreString(timeStr, 120, 120, 2);
            strcpy(lastTimeStr, timeStr);
        }

        if (strcmp(dateStr, lastDateStr) != 0)
        {
            display->fillRect(11, 180, 218, 32, TFT_BLACK);
            display->setTextColor(TFT_WHITE);
            display->setTextSize(2);
            display->drawCentreString(dateStr, 120, 180, 2);
            strcpy(lastDateStr, dateStr);
        }
    }
}