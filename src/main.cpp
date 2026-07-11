#include <Arduino.h>
#include <time.h>

#include "LGFX_Config.h"
#include "LGFX_Config2.h"

#include "ClockApp.h"
#include "QuotesApp.h"
#include "SettingsApp.h"

#include "RotaryManager.h"
#include "AppManager.h"
#include "DisplayManager.h"
#include "WiFiManager.h"
#include "secrets.h"
#include "StatusBar.h"
#include "AnimationManager.h"
#include "LauncherApp.h"
#include "NotificationManager.h"

#define DEBUG 0

#if DEBUG
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif

LGFX display1;
LGFX2 display2;

ClockApp clockApp;
QuotesApp quotesApp;
SettingsApp settingsApp;

AppManager appManager;
DisplayManager displayManager;
WiFiManager wifiManager;

RotaryManager rotary(32, 33, 25);
AnimationManager animationManager;

enum class NavigationMode
{
    LAUNCHER,
    APP
};

struct DisplayState
{
    NavigationMode mode;
    int selectedAppIdx;
    int runningAppIdx;
};

DisplayState ds1 = { NavigationMode::APP, 0, 0 };
DisplayState ds2 = { NavigationMode::APP, 1, 1 };

LauncherApp launcherApp1;
LauncherApp launcherApp1_next;
LauncherApp launcherApp2;
LauncherApp launcherApp2_next;

int selectedDisplay = 0; // 0 = Left, 1 = Right

void redrawDisplays()
{
    App* leftApp = (ds1.mode == NavigationMode::LAUNCHER) ? (App*)&launcherApp1 : appManager.getApp(ds1.runningAppIdx);
    App* rightApp = (ds2.mode == NavigationMode::LAUNCHER) ? (App*)&launcherApp2 : appManager.getApp(ds2.runningAppIdx);

    displayManager.redrawDisplays(
        &display1,
        &display2,
        leftApp,
        rightApp,
        selectedDisplay
    );
}

void setup()
{
#if DEBUG
    Serial.begin(115200);
#endif

    display1.init();
    display1.setRotation(2);

    display2.init();
    display2.setRotation(2);

    rotary.begin();
    wifiManager.begin(WIFI_SSID, WIFI_PASSWORD);

    configTzTime("IST-5:30", "pool.ntp.org", "time.nist.gov");

    appManager.addApp(&clockApp);
    appManager.addApp(&quotesApp);
    appManager.addApp(&settingsApp);

    // Sync initial indices
    launcherApp1.setSelectedIdx(ds1.selectedAppIdx);
    launcherApp2.setSelectedIdx(ds2.selectedAppIdx);

    redrawDisplays();

    // Post initial mock notifications to demonstrate sliding popups at boot
    NotificationManager::postNotification(&display1, "Weather Updated", ThemeManager::current().success, 4, 3000);
    NotificationManager::postNotification(&display2, "Battery Low", ThemeManager::current().warning, 1, 3000);
}

void loop()
{
    wifiManager.update();

    if (animationManager.isAnimating())
    {
        animationManager.update();
        return;
    }

    App* app1 = (ds1.mode == NavigationMode::LAUNCHER) ? (App*)&launcherApp1 : appManager.getApp(ds1.runningAppIdx);
    App* app2 = (ds2.mode == NavigationMode::LAUNCHER) ? (App*)&launcherApp2 : appManager.getApp(ds2.runningAppIdx);

    if (ds1.mode == NavigationMode::APP)
        app1->update(&display1);
    if (ds2.mode == NavigationMode::APP)
        app2->update(&display2);

    StatusBar::draw(&display1, app1->getName());
    StatusBar::draw(&display2, app2->getName());

    // Update notification animations
    NotificationManager::update();

    // Auto-post notification on WiFi connection transition
    static bool lastWifiConnected = false;
    bool currentWifiConnected = wifiManager.isConnected();
    if (currentWifiConnected && !lastWifiConnected)
    {
        NotificationManager::postNotification(&display1, "WiFi Connected", ThemeManager::current().success, 0);
        NotificationManager::postNotification(&display2, "WiFi Connected", ThemeManager::current().success, 0);
    }
    lastWifiConnected = currentWifiConnected;

    int rotation = rotary.getRotation();

    if (rotary.isLongPressed())
    {
        if (selectedDisplay == 0)
        {
            if (ds1.mode == NavigationMode::APP)
            {
                ds1.mode = NavigationMode::LAUNCHER;
                ds1.selectedAppIdx = ds1.runningAppIdx;
                launcherApp1.setSelectedIdx(ds1.selectedAppIdx);

                animationManager.startAnimation(
                    &display1,
                    appManager.getApp(ds1.runningAppIdx),
                    &launcherApp1,
                    AnimationType::FADE
                );
            }
            else
            {
                selectedDisplay = 1;
                redrawDisplays();
            }
        }
        else
        {
            if (ds2.mode == NavigationMode::APP)
            {
                ds2.mode = NavigationMode::LAUNCHER;
                ds2.selectedAppIdx = ds2.runningAppIdx;
                launcherApp2.setSelectedIdx(ds2.selectedAppIdx);

                animationManager.startAnimation(
                    &display2,
                    appManager.getApp(ds2.runningAppIdx),
                    &launcherApp2,
                    AnimationType::FADE
                );
            }
            else
            {
                selectedDisplay = 0;
                redrawDisplays();
            }
        }
        return;
    }

    if (rotary.isPressed())
    {
        if (selectedDisplay == 0)
        {
            if (ds1.mode == NavigationMode::LAUNCHER)
            {
                ds1.mode = NavigationMode::APP;
                ds1.runningAppIdx = ds1.selectedAppIdx;

                animationManager.startAnimation(
                    &display1,
                    &launcherApp1,
                    appManager.getApp(ds1.runningAppIdx),
                    AnimationType::FADE
                );
            }
            else
            {
                app1->onClick();
            }
        }
        else
        {
            if (ds2.mode == NavigationMode::LAUNCHER)
            {
                ds2.mode = NavigationMode::APP;
                ds2.runningAppIdx = ds2.selectedAppIdx;

                animationManager.startAnimation(
                    &display2,
                    &launcherApp2,
                    appManager.getApp(ds2.runningAppIdx),
                    AnimationType::FADE
                );
            }
            else
            {
                app2->onClick();
            }
        }
        return;
    }

    if (rotation != 0)
    {
        int count = appManager.getCount();
        if (selectedDisplay == 0)
        {
            if (ds1.mode == NavigationMode::LAUNCHER)
            {
                int oldIdx = ds1.selectedAppIdx;
                ds1.selectedAppIdx = (ds1.selectedAppIdx + rotation + count) % count;

                launcherApp1.setSelectedIdx(oldIdx);
                launcherApp1_next.setSelectedIdx(ds1.selectedAppIdx);

                animationManager.startAnimation(
                    &display1,
                    &launcherApp1,
                    &launcherApp1_next,
                    rotation > 0 ? AnimationType::SLIDE_LEFT : AnimationType::SLIDE_RIGHT
                );

                launcherApp1.setSelectedIdx(ds1.selectedAppIdx);
            }
            else
            {
                app1->onEncoder(rotation);
            }
        }
        else
        {
            if (ds2.mode == NavigationMode::LAUNCHER)
            {
                int oldIdx = ds2.selectedAppIdx;
                ds2.selectedAppIdx = (ds2.selectedAppIdx + rotation + count) % count;

                launcherApp2.setSelectedIdx(oldIdx);
                launcherApp2_next.setSelectedIdx(ds2.selectedAppIdx);

                animationManager.startAnimation(
                    &display2,
                    &launcherApp2,
                    &launcherApp2_next,
                    rotation > 0 ? AnimationType::SLIDE_LEFT : AnimationType::SLIDE_RIGHT
                );

                launcherApp2.setSelectedIdx(ds2.selectedAppIdx);
            }
            else
            {
                app2->onEncoder(rotation);
            }
        }
    }
}
