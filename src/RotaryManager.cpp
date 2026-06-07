#include "RotaryManager.h"
#include <Arduino.h>

RotaryManager::RotaryManager(int clk, int dt, int sw)
{
    clkPin = clk;
    dtPin = dt;
    swPin = sw;
}

void RotaryManager::begin()
{
    pinMode(clkPin, INPUT);
    pinMode(dtPin, INPUT);
    pinMode(swPin, INPUT_PULLUP);

    lastCLK = digitalRead(clkPin);
}

int RotaryManager::getRotation()
{
    static unsigned long lastRotation = 0;

    int currentCLK = digitalRead(clkPin);

    if (currentCLK != lastCLK &&
        millis() - lastRotation > 50)
    {
        lastRotation = millis();

        int direction;

        if (digitalRead(dtPin) != currentCLK)
            direction = 1;
        else
            direction = -1;

        lastCLK = currentCLK;

        return direction;
    }

    lastCLK = currentCLK;

    return 0;
}

bool RotaryManager::isPressed()
{
    static bool lastState = HIGH;
    static unsigned long lastDebounce = 0;

    bool currentState = digitalRead(swPin);

    if (currentState != lastState)
    {
        lastDebounce = millis();
    }

    if (millis() - lastDebounce > 200)
    {
        if (lastState == HIGH &&
            currentState == LOW)
        {
            lastState = currentState;
            return true;
        }
    }

    lastState = currentState;

    return false;
}

bool RotaryManager::isLongPressed()
{
    static bool triggered = false;
    static unsigned long pressStart = 0;

    if (digitalRead(swPin) == LOW)
    {
        if (pressStart == 0)
            pressStart = millis();

        if (!triggered &&
            millis() - pressStart > 1000)
        {
            triggered = true;
            return true;
        }
    }
    else
    {
        pressStart = 0;
        triggered = false;
    }

    return false;
}