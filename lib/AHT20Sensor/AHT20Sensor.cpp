#include <Arduino.h>
#include "AHT20Sensor.h"

AHT20Sensor::AHT20Sensor(uint8_t i2c_address)
{
    _aht = new Adafruit_AHTX0();
    _i2c_address = i2c_address;    
    isConnected = false;
}

bool AHT20Sensor::connect(bool connectionProbe, bool waitUntilConnected){
    while (! _aht->begin()){
        if(!waitUntilConnected) {
            isConnected = false;
            return false;
        }
    }

    isConnected = true;
    if(connectionProbe && !readData().isDataReceived)isConnected = false;

    return true;    
}

AHT20Data AHT20Sensor::readData()
{
    AHT20Data data { false };    

    if(this->isConnected) {
        data.isDataReceived = true;

        sensors_event_t humidity, temp;
        _aht->getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
        data.temperatureCelsium = temp.temperature;
        data.humidity = humidity.relative_humidity;

        if(isnan(data.temperatureCelsium) || 
            data.temperatureCelsium < -100 ||
            data.temperatureCelsium > 200)data.isDataReceived = false;
    }

    return data;
}