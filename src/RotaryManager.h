#pragma once

class RotaryManager
{
private:
    int clkPin;
    int dtPin;
    int swPin;

    int lastCLK;
    unsigned long lastMove = 0;

public:
    RotaryManager(int clk, int dt, int sw);

    void begin();

    int getRotation();

    bool isPressed();

    bool isLongPressed();
};