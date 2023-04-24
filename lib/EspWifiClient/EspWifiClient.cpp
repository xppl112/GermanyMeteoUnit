#include "EspWifiClient.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

EspWifiClient::EspWifiClient(){
    _wifiStatus = WL_IDLE_STATUS;
    isWifiConnected = false;

    wiFiClient = new WiFiClient();
}

bool EspWifiClient::connectWifi(const char* ssid, const char* password, uint8_t timeoutSeconds){
    _wifiSsid = ssid;
    _wifiPassword = password;
    isWifiConnected = false;

    // Use static IP to save time
    /*
    IPAddress ip( 192, 168, 0, 1 );
    IPAddress gateway( 192, 168, 0, 254 );
    IPAddress subnet( 255, 255, 255, 0 );
    WiFi.config( ip, gateway, subnet );
    WiFi.begin...
    */

    WiFi.persistent(false);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    _wifiStatus = WiFi.waitForConnectResult(timeoutSeconds * 1000);

    isWifiConnected = WiFi.isConnected();
    currentSignalLevel = WiFi.RSSI();
    return isWifiConnected;
}

void EspWifiClient::disconnectWifi() {
    _wifiStatus = WiFi.disconnect(true);
    delay(1);
    isWifiConnected = false;
}

HttpResponse EspWifiClient::sendGetRequest(String host, String resource, uint8_t timeoutSeconds){
    HttpResponse httpResponse;

    HTTPClient client;
    client.begin(*wiFiClient, host + resource);
    client.setTimeout(timeoutSeconds * 1000);

    int resultCode = client.GET();

    if(resultCode != -1){
        httpResponse.statusCode = resultCode;
        
        if(resultCode == HTTP_CODE_OK){
            httpResponse.payload = client.getString();
        }
        else {
            httpResponse.payload = client.errorToString(resultCode).c_str();
        }        
    }

    client.end();

    httpResponse.success = resultCode != -1 ? true : false;
    return httpResponse;
}

HttpResponse EspWifiClient::sendPostJsonRequest(String host, String resource, String jsonPayload, uint8_t timeoutSeconds){
    HttpResponse httpResponse;

    HTTPClient client;
    client.begin(*wiFiClient, host + resource);
    client.addHeader("Content-Type", "application/json");
    client.setTimeout(timeoutSeconds * 1000);   

    int resultCode = client.POST(jsonPayload);
    if(resultCode != -1){
        httpResponse.statusCode = resultCode;
        
        if(resultCode == HTTP_CODE_OK){
            httpResponse.payload = client.getString();
        }
        else {
            httpResponse.payload = client.errorToString(resultCode).c_str();
        }        
    }

    client.end();

    httpResponse.success = resultCode != -1 ? true : false;
    return httpResponse;
}