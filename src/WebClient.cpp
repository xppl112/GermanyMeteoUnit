#include "WebClient.h"
#include "config.h"

//#define DEBUG

WebClient::WebClient(){
    _client = new EspWifiClient();
}

bool WebClient::connect(){
    if(!_client->connectWifi(_config.WifiSSID, _config.WifiPassword, _config.WifiConnectionTimeout))return false;
    lastSignalLevel = _client->currentSignalLevel;
    
    #ifdef DEBUG
    Serial.print("Wifi Level: ");Serial.println(lastSignalLevel);
    #endif

    return true;
}

bool WebClient::disconnect(){
    _client->disconnectWifi();
    return true;
}

bool WebClient::postData(PostData data, GetData* gdata){
    auto jsonData = data.toJson();

    #ifdef DEBUG
    Serial.print("Json request: ");Serial.println(jsonData);
    #endif

    auto response = _client->sendPostJsonRequest(
        _config.ServerHost, _config.ServerApiPostWeatherDataUrl, jsonData, _config.HttpRequestTimeout);
    
    #ifdef DEBUG
    Serial.print("Http code: ");Serial.println(response.statusCode);
    #endif
    
    if(response.success && response.statusCode == 200)
    {
        #ifdef DEBUG
        Serial.print("Json response: ");Serial.println(response.payload);
        #endif

        auto parsedResponse = GetData::constructFromJson(response.payload);
        gdata->dataReceived = parsedResponse.dataReceived;
        gdata->sleepDurationSeconds = parsedResponse.sleepDurationSeconds;
        gdata->measureAirQualityCycle = parsedResponse.measureAirQualityCycle;
        gdata->airQualityMeasurementDurationSeconds = parsedResponse.airQualityMeasurementDurationSeconds;
        return true;
    }

    return false;
}