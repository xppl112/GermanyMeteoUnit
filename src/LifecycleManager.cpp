#include "LifecycleManager.h"
#include "config.h"

extern void safeDelay(unsigned int ms);
//#define DEBUG

LifecycleManager::LifecycleManager() {
    _powerManager = new PowerManager();
    _sensors = new Sensors();
    _webClient = new WebClient();
    _storage = new Storage();

    pinMode(CONTROL_LED_PIN, OUTPUT);digitalWrite(CONTROL_LED_PIN, HIGH);
    initialize();
}

void LifecycleManager::initialize(){
    #ifdef DEBUG
    Serial.println("LifecycleManager::initialize");
    #endif

    _systemState = _storage->getSystemState();
    _settings = _storage->getSettings();
    _storage->incrementStartupCounter();

    blinkControlLed(1);
}

void LifecycleManager::iterate() {
    #ifdef DEBUG
    Serial.println("LifecycleManager::iterate");
    #endif

    measure();
    sendData();
    updateSystemState();
    sleep();
}

void LifecycleManager::measure(){
    #ifdef DEBUG
    Serial.println("LifecycleManager::measure");
    #endif

    bool useHighCurrentSensors = _systemState.bootCounter % _settings.measureAirQualityCycle == 0;

    _sensors->connect(SensorType::LOW_CURRENT);
    _idlePowerLevels = _sensors->getPowerLevels();

    if(useHighCurrentSensors){
        _sensors->connect(SensorType::HIGH_CURRENT);
    }

    _currentWeather = _sensors->getWeather(_settings.airQualityMeasurementDurationSeconds);
    if(useHighCurrentSensors){
        _airSensorsPowerLevels = _sensors->getPowerLevels();
        _sensors->disconnect(SensorType::HIGH_CURRENT);
    }
}

void LifecycleManager::sendData(){
    #ifdef DEBUG
    Serial.println("LifecycleManager::sendData");
    #endif

    PostData data = {
        //meteo sensors
        .ahtTemperature = _currentWeather.ahtTemperature,
        .ahtHumidity = _currentWeather.ahtHumidity,
        .bmpTemperature = _currentWeather.bmpTemperature,
        .pressure = _currentWeather.pressure,
        .raindropLevel = _currentWeather.raindropLevel,
        .isAirQualityMeasured = _currentWeather.isAirQualityMeasured,

        //power
        .batteryVoltageIdle = _idlePowerLevels.batteryVoltage,
        .batteryCurrentIdle = _idlePowerLevels.batteryCurrent,

        .bootCount = _systemState.bootCounter
    };

    if(data.isAirQualityMeasured){
        data.pm1_0 = _currentWeather.pm1_0;
        data.pm2_5 = _currentWeather.pm2_5;
        data.pm10_0 = _currentWeather.pm10_0;
        data.gasResistance = _currentWeather.gasResistance;
        data.bmeTemperature = _currentWeather.bmeTemperature;
        data.bmeHumidity = _currentWeather.bmeHumidity;
        data.bmePressure = _currentWeather.bmePressure;

        data.batteryVoltageHighSensorsOn = _airSensorsPowerLevels.batteryVoltage;
        data.batteryCurrentHighSensorsOn = _airSensorsPowerLevels.batteryCurrent;
    }

    _powerManager->powerRF(true);
    for(int i = WIFI_CONNECTION_RETRY_ATTEMPTS_COUNT; i > 0 ; i--){
        if(!_webClient->connect()){
            _systemState.wifiConnectionErrors++;
            if(i == 1){
                blinkControlLed(4);
                return;
            }
        }
        else break;
    }

    //Measure current with wifi connected
    _wifiPowerLevels = _sensors->getPowerLevels();
    _sensors->disconnect(SensorType::LOW_CURRENT);

    data.batteryVoltageWifiOn = _wifiPowerLevels.batteryVoltage;
    data.batteryCurrentWifiOn = _wifiPowerLevels.batteryCurrent;
    data.wifiSignalLevel = _webClient->lastSignalLevel;
    data.wifiConnectionErrorsCount = _systemState.wifiConnectionErrors;
    data.wifiSendingErrorsCount = _systemState.wifiSendingErrors;

    GetData gdata;
    for(int i = WIFI_SEND_DATA_ATTEMPTS_COUNT; i > 0; i--){
        if(!_webClient->postData(data, &gdata)){
            _systemState.wifiSendingErrors++;
            blinkControlLed(3);
        }
        else {
            if(gdata.dataReceived && gdata.sleepDurationSeconds > 0 && gdata.sleepDurationSeconds < 10000 &&
               gdata.measureAirQualityCycle > 0 && gdata.measureAirQualityCycle < 100 &&
               gdata.airQualityMeasurementDurationSeconds > 0 && gdata.airQualityMeasurementDurationSeconds < 1000){
                if(_settings.sleepDurationSeconds != gdata.sleepDurationSeconds ||
                _settings.measureAirQualityCycle != gdata.measureAirQualityCycle ||
                _settings.airQualityMeasurementDurationSeconds != gdata.airQualityMeasurementDurationSeconds)
                    _isSettingsUpdated = true;

                _settings.sleepDurationSeconds = gdata.sleepDurationSeconds;
                _settings.measureAirQualityCycle = gdata.measureAirQualityCycle;
                _settings.airQualityMeasurementDurationSeconds = gdata.airQualityMeasurementDurationSeconds;
                blinkControlLed(2);
                break;
            }
        }
        data.wifiSendingErrorsCount = _systemState.wifiSendingErrors;
    }
    _webClient->disconnect();
}

void LifecycleManager::updateSystemState(){    
    if(_isSettingsUpdated)_storage->updateSettings(_settings);
    _storage->updateSystemState(_systemState);
}

void LifecycleManager::sleep(){
    _powerManager->deepSleep(_settings.sleepDurationSeconds);
}

void LifecycleManager::blinkControlLed(int count){
    for(int i=0;i<count;i++){
        if(i!=0)safeDelay(300);
        digitalWrite(CONTROL_LED_PIN, LOW);
        safeDelay(300);
        digitalWrite(CONTROL_LED_PIN, HIGH);
    }
}