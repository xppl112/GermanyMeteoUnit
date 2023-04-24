#include <Arduino.h>
#include "BMP280Sensor.h"

BMP280Sensor::BMP280Sensor(uint8_t i2c_address)
{
    _bmp = new Adafruit_BMP280();
    _i2c_address = i2c_address;    
    isConnected = false;
}

bool BMP280Sensor::connect(bool connectionProbe, bool waitUntilConnected){
    while (! _bmp->begin(_i2c_address)){
        if(!waitUntilConnected) {
            isConnected = false;
            return false;
        }
    }

    isConnected = true;
    if(connectionProbe && !readData().isDataReceived)isConnected = false;

    return true;    
}

BMP280Data BMP280Sensor::readData()
{
    BMP280Data data { false };    

    if(this->isConnected) {
        data.isDataReceived = true;
        data.temperatureCelsium = _bmp->readTemperature();
        data.pressureInHPascals = _bmp->readPressure() / 100.0F;

        if(isnan(data.temperatureCelsium) || 
            data.temperatureCelsium < -100 ||
            data.temperatureCelsium > 200)data.isDataReceived = false;
    }

    return data;
}