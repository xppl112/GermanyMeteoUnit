#include <Arduino.h>
#include "BME680Sensor.h"

BME680Sensor::BME680Sensor(uint8_t i2c_address)
{
    _bme = new Adafruit_BME680();
    _i2c_address = i2c_address;    
    isConnected = false;
}

bool BME680Sensor::connect(bool connectionProbe, bool waitUntilConnected){
    while (! _bme->begin(_i2c_address)){
        if(!waitUntilConnected) {
            isConnected = false;
            return false;
        }
    }
      // Set up oversampling and filter initialization
    _bme->setTemperatureOversampling(BME680_OS_8X);
    _bme->setHumidityOversampling(BME680_OS_2X);
    _bme->setPressureOversampling(BME680_OS_4X);
    _bme->setIIRFilterSize(BME680_FILTER_SIZE_3);
    _bme->setGasHeater(320, 150); // 320*C for 150 ms

    isConnected = true;
    if(connectionProbe && !readData().isDataReceived)isConnected = false;

    return true;    
}

BME680Data BME680Sensor::readData()
{
    BME680Data data { false };    

    //if(this->isConnected) {
        

        data.isDataReceived = _bme->performReading();
        data.temperatureCelsium = _bme->temperature;
        data.humidity = _bme->humidity;
        data.pressureInHPascals = _bme->pressure / 100.0F;
        data.gasResistance = _bme->gas_resistance / 1000.0;

    //    if(isnan(data.temperatureCelsium) || 
    //        data.temperatureCelsium < -100 ||
    //        data.temperatureCelsium > 200)data.isDataReceived = false;
    //}

    return data;
}