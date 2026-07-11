#pragma once

#include <Arduino.h>

struct Settings
{
    uint8_t brightness;
    int themeIdx;
    char wifiSSID[33];
    char wifiPassword[65];
    int leftAppIdx;
    int rightAppIdx;
    bool autoReconnect;
};

class SettingsManager
{
private:
    static Settings currentSettings;

public:
    static const Settings& get();
    static void set(const Settings& settings);
    static void load();
    static void save();
    static void applySettings();
};
