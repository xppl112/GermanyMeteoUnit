#pragma once
#include <Arduino.h>
#include <Models/Settings.h>
#include <Models/SystemState.h>
#include <Preferences.h>

class Storage
{
public:
    Storage();
    
    Settings getSettings();
    void updateSettings(Settings settings);

    SystemState getSystemState();
    void incrementStartupCounter();
    void updateSystemState(SystemState state);

private:
    Preferences _preferences;
    const char* _appName = "app";
};