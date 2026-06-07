#include <Arduino.h>

#include "LGFX_Config.h"
#include "LGFX_Config2.h"

#include "ClockApp.h"
#include "QuotesApp.h"
#include "SettingsApp.h"

#include "RotaryManager.h"
#include "AppManager.h"

LGFX display1;
LGFX2 display2;

ClockApp clockApp;
QuotesApp quotesApp;
SettingsApp settingsApp;

AppManager appManager;

RotaryManager rotary(32, 33, 25);

int selectedDisplay = 0; // 0 = Left, 1 = Right

int display1App = 0;
int display2App = 1;

bool selectingDisplay = false;
int displayMenuIndex = 0;

void redrawDisplays()
{
    appManager.getApp(display1App)->draw(&display1);
    appManager.getApp(display2App)->draw(&display2);
}

void showDisplaySelector()
{
    display1.fillScreen(TFT_BLACK);

    display1.setTextColor(TFT_CYAN);
    display1.setTextSize(2);

    display1.drawCentreString(
        "SELECT DISPLAY",
        120,
        40,
        2
    );

    if (displayMenuIndex == 0)
    {
        display1.setTextColor(TFT_GREEN);
        display1.drawCentreString("> LEFT <", 120, 120, 2);

        display1.setTextColor(TFT_WHITE);
        display1.drawCentreString("RIGHT", 120, 180, 2);
    }
    else
    {
        display1.setTextColor(TFT_WHITE);
        display1.drawCentreString("LEFT", 120, 120, 2);

        display1.setTextColor(TFT_GREEN);
        display1.drawCentreString("> RIGHT <", 120, 180, 2);
    }
}

void setup()
{
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

    if (selectingDisplay)
    {
        if (rotation != 0)
        {
            displayMenuIndex = !displayMenuIndex;
            showDisplaySelector();
        }

        if (rotary.isPressed())
        {
            selectedDisplay = displayMenuIndex;

            selectingDisplay = false;

            redrawDisplays();

            delay(300);
        }

        return;
    }

    if (rotary.isLongPressed())
    {
        selectingDisplay = true;
        showDisplaySelector();
        return;
    }

    if (rotation != 0)
    {
        Serial.print("Rotation: ");
        Serial.println(rotation);

        if (selectedDisplay == 0)
        {
            display1App += rotation;

            if (display1App < 0)
                display1App = appManager.getCount() - 1;

            if (display1App >= appManager.getCount())
                display1App = 0;
        }
        else
        {
            display2App += rotation;

            if (display2App < 0)
                display2App = appManager.getCount() - 1;

            if (display2App >= appManager.getCount())
                display2App = 0;
        }
        Serial.print("Display1 App: ");
        Serial.println(display1App);

        Serial.print("Display2 App: ");
        Serial.println(display2App);
        redrawDisplays();
    }
}