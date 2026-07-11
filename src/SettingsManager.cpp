#include "SettingsManager.h"
#include "ThemeManager.h"
#include "secrets.h"
#include "LGFX_Config.h"
#include "LGFX_Config2.h"
#include <Preferences.h>

extern LGFX display1;
extern LGFX2 display2;

Settings SettingsManager::currentSettings;

const Settings& SettingsManager::get()
{
    return currentSettings;
}

void SettingsManager::set(const Settings& settings)
{
    currentSettings = settings;
    save();
    applySettings();
}

void SettingsManager::load()
{
    Preferences prefs;
    prefs.begin("settings", false);

    currentSettings.brightness = prefs.getUChar("brightness", 128); // default medium brightness
    currentSettings.themeIdx = prefs.getInt("themeIdx", 0); // default Dark Theme
    
    // Load WiFi credentials with secrets.h fallback
    String ssid = prefs.getString("wifiSSID", WIFI_SSID);
    String pass = prefs.getString("wifiPassword", WIFI_PASSWORD);
    
    strncpy(currentSettings.wifiSSID, ssid.c_str(), sizeof(currentSettings.wifiSSID) - 1);
    currentSettings.wifiSSID[sizeof(currentSettings.wifiSSID) - 1] = '\0';
    
    strncpy(currentSettings.wifiPassword, pass.c_str(), sizeof(currentSettings.wifiPassword) - 1);
    currentSettings.wifiPassword[sizeof(currentSettings.wifiPassword) - 1] = '\0';

    currentSettings.leftAppIdx = prefs.getInt("leftAppIdx", 0); // default ClockApp
    currentSettings.rightAppIdx = prefs.getInt("rightAppIdx", 1); // default QuotesApp
    currentSettings.autoReconnect = prefs.getBool("autoReconnect", true);

    prefs.end();
}

void SettingsManager::save()
{
    Preferences prefs;
    prefs.begin("settings", false);

    prefs.putUChar("brightness", currentSettings.brightness);
    prefs.putInt("themeIdx", currentSettings.themeIdx);
    prefs.putString("wifiSSID", currentSettings.wifiSSID);
    prefs.putString("wifiPassword", currentSettings.wifiPassword);
    prefs.putInt("leftAppIdx", currentSettings.leftAppIdx);
    prefs.putInt("rightAppIdx", currentSettings.rightAppIdx);
    prefs.putBool("autoReconnect", currentSettings.autoReconnect);

    prefs.end();
}

void SettingsManager::applySettings()
{
    // Apply Theme
    ThemeManager::setTheme(currentSettings.themeIdx);

    // Apply brightness to displays
    display1.setBrightness(currentSettings.brightness);
    display2.setBrightness(currentSettings.brightness);
}
