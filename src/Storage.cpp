#include <Storage.h>

//#define DEBUG

Storage::Storage(){
}

Settings Storage::getSettings(){
    _preferences.begin(_appName, true);
    Settings settings;
    settings.sleepDurationSeconds = _preferences.getUInt("s_sds", DEFAULT_SETTINGS.sleepDurationSeconds);
    settings.measureAirQualityCycle = _preferences.getUChar("s_maqc", DEFAULT_SETTINGS.measureAirQualityCycle);
    settings.airQualityMeasurementDurationSeconds = _preferences.getUInt("s_aqmd", DEFAULT_SETTINGS.airQualityMeasurementDurationSeconds);
    _preferences.end();

    #ifdef DEBUG
    Serial.print("GetSettings: ");Serial.print(settings.sleepDurationSeconds);Serial.print("; ");
    Serial.print(settings.measureAirQualityCycle);Serial.print("; ");
    Serial.print(settings.airQualityMeasurementDurationSeconds);Serial.println("; ");
    #endif

    return settings;
}

void Storage::updateSettings(Settings settings){
    _preferences.begin(_appName, false);
    _preferences.putUInt("s_sds", settings.sleepDurationSeconds);
    _preferences.putUChar("s_maqc", settings.measureAirQualityCycle);
    _preferences.putUInt("s_aqmd", settings.airQualityMeasurementDurationSeconds);
    _preferences.end();

    #ifdef DEBUG
    Serial.print("UpdateSettings: ");Serial.print(settings.sleepDurationSeconds);Serial.print("; ");
    Serial.print(settings.measureAirQualityCycle);Serial.print("; ");
    Serial.print(settings.airQualityMeasurementDurationSeconds);Serial.println("; ");
    #endif
}

SystemState Storage::getSystemState(){
    SystemState state;
    if(!ESP.rtcUserMemoryRead(0, &state.bootCounter, sizeof(state.bootCounter)))state.bootCounter = 0;
    if(!ESP.rtcUserMemoryRead(4, &state.wifiConnectionErrors, sizeof(state.wifiConnectionErrors)))state.wifiConnectionErrors = 0;
    if(!ESP.rtcUserMemoryRead(8, &state.wifiSendingErrors, sizeof(state.wifiSendingErrors)))state.wifiSendingErrors = 0;

    if(state.bootCounter > 10000000)state.bootCounter = 0;
    if(state.wifiConnectionErrors > 10000000)state.wifiConnectionErrors = 0;
    if(state.wifiSendingErrors > 10000000)state.wifiSendingErrors = 0;

    #ifdef DEBUG
    Serial.print("GetSystemState: ");Serial.print(state.bootCounter);Serial.print("; ");
    Serial.print(state.wifiConnectionErrors);Serial.print("; ");
    Serial.print(state.wifiSendingErrors);Serial.println("; ");
    #endif

    return state;
}

void Storage::incrementStartupCounter(){
    uint32_t bootCounter;
    if(!ESP.rtcUserMemoryRead(0, &bootCounter, sizeof(bootCounter)))bootCounter = 0;
    if(bootCounter > 10000000)bootCounter = 0;
    bootCounter++;

    #ifdef DEBUG
    Serial.print("incrementStartupCounter to ");Serial.print(bootCounter);Serial.print(": ");
    #endif

    if(!ESP.rtcUserMemoryWrite(0, &bootCounter, sizeof(bootCounter))) {
        //do nothing
        #ifdef DEBUG
        Serial.println("ERROR");
        #endif
    }
    else {
        #ifdef DEBUG
        Serial.println("OK");
        #endif
    }
}

void Storage::updateSystemState(SystemState state){
    if(!ESP.rtcUserMemoryWrite(4, &state.wifiConnectionErrors, sizeof(state.wifiConnectionErrors))) {
            //do nothing
    }
    if(!ESP.rtcUserMemoryWrite(8, &state.wifiSendingErrors, sizeof(state.wifiSendingErrors))) {
            //do nothing
    }
}