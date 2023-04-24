#pragma once

#include <ArduinoJson.h>

struct GetData {
    bool dataReceived;
    uint16_t sleepDurationSeconds;
    uint8_t measureAirQualityCycle;
    uint16_t airQualityMeasurementDurationSeconds;

    static GetData constructFromJson(String json){
        GetData returnValue {.dataReceived = false};
        StaticJsonDocument<300> responseJson;
        deserializeJson(responseJson, json);

        if(responseJson.containsKey("sds") && responseJson.containsKey("maqc") && responseJson.containsKey("aqmds")){
            returnValue.dataReceived = true;
            returnValue.sleepDurationSeconds = responseJson["sds"];
            returnValue.measureAirQualityCycle = responseJson["maqc"];
            returnValue.airQualityMeasurementDurationSeconds = responseJson["aqmds"];
        }
        return returnValue;
    }
};