#pragma once
#include "PowerManager.h"
#include "Sensors.h"
#include "WebClient.h"
#include "Storage.h"
#include "Models/SystemState.h"
#include "Models/Settings.h"

class LifecycleManager
{
public:
    LifecycleManager();
    void iterate();

    void initialize();
    void measure();
    void sendData();
    void getUpdates();

private:
    Settings _settings;
    SystemState _systemState;
    bool _isSettingsUpdated = false;

    PowerManager* _powerManager;
    Sensors* _sensors;
    WebClient* _webClient;
    Storage* _storage;

    Weather _currentWeather;
    PowerLevels _idlePowerLevels;
    PowerLevels _airSensorsPowerLevels;
    PowerLevels _wifiPowerLevels;

    void updateSystemState();
    void sleep();
    void blinkControlLed(int count);
};