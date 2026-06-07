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
    lastEncoded = readEncoderState();
    lastButtonReading = digitalRead(swPin);
    buttonState = lastButtonReading;
}

int RotaryManager::readEncoderState()
{
    return (digitalRead(clkPin) << 1) | digitalRead(dtPin);
}

int RotaryManager::getRotation()
{
    const unsigned long rotationRateLimitMs = 35;
    int currentEncoded = readEncoderState();

    if (currentEncoded == lastEncoded)
        return 0;

    int transition = (lastEncoded << 2) | currentEncoded;
    int step = 0;

    switch (transition)
    {
        case 0b0001:
        case 0b0111:
        case 0b1110:
        case 0b1000:
            step = -1;
            break;

        case 0b0010:
        case 0b1011:
        case 0b1101:
        case 0b0100:
            step = 1;
            break;

        default:
            encoderDelta = 0;
            lastEncoded = currentEncoded;
            return 0;
    }

    lastEncoded = currentEncoded;
    encoderDelta += step;

    if (encoderDelta >= 4 || encoderDelta <= -4)
    {
        unsigned long now = millis();
        int direction = encoderDelta > 0 ? 1 : -1;

        encoderDelta = 0;

        if (lastRotationEvent != 0 &&
            now - lastRotationEvent < rotationRateLimitMs)
        {
            lastRotationEvent = now;
            return 0;
        }

        lastRotationEvent = now;
        return direction;
    }

    return 0;
}

bool RotaryManager::isPressed()
{
    bool currentReading = digitalRead(swPin);

    if (currentReading != lastButtonReading)
    {
        lastButtonDebounce = millis();
        lastButtonReading = currentReading;
    }

    if (millis() - lastButtonDebounce > 50 &&
        currentReading != buttonState)
    {
        buttonState = currentReading;

        if (buttonState == LOW)
        {
            return true;
        }
    }

    return false;
}

bool RotaryManager::isLongPressed()
{
    if (digitalRead(swPin) == LOW)
    {
        if (pressStart == 0)
            pressStart = millis();

        if (!longPressTriggered &&
            millis() - pressStart > 1000)
        {
            longPressTriggered = true;
            return true;
        }
    }
    else
    {
        pressStart = 0;
        longPressTriggered = false;
    }

    return false;
}
