#pragma once

#include <Arduino.h>

class RotaryManager
{
private:
    int clkPin;
    int dtPin;
    int swPin;

    int lastCLK;
    unsigned long lastMove = 0;
    int lastEncoded = 0;
    int encoderDelta = 0;
    unsigned long lastRotationEvent = 0;

    bool lastButtonReading = HIGH;
    bool buttonState = HIGH;
    unsigned long lastButtonDebounce = 0;
    unsigned long pressStart = 0;
    bool longPressTriggered = false;

    int readEncoderState();

public:
    RotaryManager(int clk, int dt, int sw);

    void begin();

    int getRotation();

    bool isPressed();

    bool isLongPressed();
};
