#pragma once

#include <ArduinoJson.h>

struct PostData {
    //meteo sensors
    float ahtTemperature;
    int ahtHumidity;
    float bmpTemperature;
    float pressure;
    int raindropLevel;

    //air-quality sensors
    bool isAirQualityMeasured;
    int pm1_0;
    int pm2_5;
    int pm10_0;
    float gasResistance;
    float bmeTemperature;
    int bmeHumidity;
    float bmePressure;

    //power
    float batteryVoltageIdle;
    float batteryVoltageHighSensorsOn;
    float batteryVoltageWifiOn;
    float batteryCurrentIdle;
    float batteryCurrentHighSensorsOn;
    float batteryCurrentWifiOn;

    //system status
    int wifiSignalLevel;
    unsigned int wifiConnectionErrorsCount;
    unsigned int wifiSendingErrorsCount;
    unsigned int bootCount;

    String toJson(){
        DynamicJsonDocument doc(2048);

        doc["t_aht"] = ahtTemperature;
        doc["h_aht"] = ahtHumidity;
        doc["t_bmp"] = bmpTemperature;
        doc["p"] = pressure;
        doc["rd"] = raindropLevel;

        doc["iaqm"] = isAirQualityMeasured ? 1 : 0;
        if(isAirQualityMeasured){
            doc["pm_1"] = pm1_0;
            doc["pm_25"] = pm2_5;
            doc["pm_10"] = pm10_0;
            doc["gr"] = gasResistance;
            doc["t_bme"] = bmeTemperature;
            doc["h_bme"] = bmeHumidity;
            doc["p_bme"] = bmePressure;
        }

        doc["bvi"] = batteryVoltageIdle;
        doc["bci"] = batteryCurrentIdle;    
        if(isAirQualityMeasured){
            doc["bvs"] = batteryVoltageHighSensorsOn;
            doc["bcs"] = batteryCurrentHighSensorsOn;
        }
        doc["bvw"] = batteryVoltageWifiOn;
        doc["bcw"] = batteryCurrentWifiOn;

        doc["bc"] = bootCount;
        doc["wsl"] = wifiSignalLevel;
        doc["wce"] = wifiConnectionErrorsCount;
        doc["wse"] = wifiSendingErrorsCount;

        String output;
        serializeJson(doc, output);
        doc.clear();
        doc.garbageCollect();

        return output;
    }
};