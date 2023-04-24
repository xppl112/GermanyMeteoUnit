#pragma once
#include "config.h"
#include "EspWifiClient.h"
#include "Models/GetData.h"
#include "Models/PostData.h"

class WebClient
{
public:
    WebClient();
    bool connect();
    bool disconnect();
    bool postData(PostData data, GetData* gdata);

    uint8_t lastSignalLevel;

private:
    EspWifiClient* _client;
    BackendClientConfig _config;
};