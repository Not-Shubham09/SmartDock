#include <Arduino.h>

#include "LGFX_Config.h"
#include "LGFX_Config2.h"

#include "ClockApp.h"
#include "QuotesApp.h"
#include "SettingsApp.h"

#include "RotaryManager.h"
#include "AppManager.h"
#include "DisplayManager.h"

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

RotaryManager rotary(32, 33, 25);

int selectedDisplay = 0; // 0 = Left, 1 = Right

int display1App = 0;
int display2App = 1;

void redrawDisplays()
{
    displayManager.redrawDisplays(
        &display1,
        &display2,
        appManager.getApp(display1App),
        appManager.getApp(display2App),
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

    appManager.addApp(&clockApp);
    appManager.addApp(&quotesApp);
    appManager.addApp(&settingsApp);

    redrawDisplays();
}

void loop()
{
    int rotation = rotary.getRotation();

    if (rotary.isLongPressed())
    {
        selectedDisplay = selectedDisplay == 0 ? 1 : 0;

        DEBUG_PRINT("Active display: ");
        DEBUG_PRINTLN(selectedDisplay == 0 ? "LEFT" : "RIGHT");

        redrawDisplays();
        return;
    }

    if (rotation != 0)
    {
        DEBUG_PRINT("Rotation: ");
        DEBUG_PRINTLN(rotation);

        if (selectedDisplay == 0)
        {
            display1App += rotation;

            if (display1App < 0)
                display1App = appManager.getCount() - 1;

            if (display1App >= appManager.getCount())
                display1App = 0;

            DEBUG_PRINT("Active display: LEFT, selected app index: ");
            DEBUG_PRINTLN(display1App);
        }
        else
        {
            display2App += rotation;

            if (display2App < 0)
                display2App = appManager.getCount() - 1;

            if (display2App >= appManager.getCount())
                display2App = 0;

            DEBUG_PRINT("Active display: RIGHT, selected app index: ");
            DEBUG_PRINTLN(display2App);
        }

        redrawDisplays();
    }
}
