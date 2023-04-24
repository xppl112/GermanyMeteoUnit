#include <stdint.h>
#include <stream.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

struct HttpResponse {
    bool success;
    int statusCode;
    String payload;
};

class EspWifiClient
{
public:
  EspWifiClient();

  bool connectWifi(const char* ssid, const char* password, uint8_t timeoutSeconds = 10);
  void disconnectWifi();  

  HttpResponse sendGetRequest(String host, String resource, uint8_t timeoutSeconds = 10);
  HttpResponse sendPostJsonRequest(String host, String resource, String jsonPayload, uint8_t timeoutSeconds = 10);

  bool isWifiConnected;
  int8_t currentSignalLevel;

private:
  String _wifiSsid;
  String _wifiPassword;
  int _wifiStatus;
  WiFiClient *wiFiClient;
};