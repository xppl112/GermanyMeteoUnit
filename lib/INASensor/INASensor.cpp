#include <Arduino.h>
#include "INASensor.h"

INASensor::INASensor(uint8_t i2c_address)
{
    _ina = new INA219_WE(i2c_address);
    _i2c_address = i2c_address;    
    isConnected = false;
}

bool INASensor::connect(bool connectionProbe, bool waitUntilConnected){
    while (! _ina->init()){
        if(!waitUntilConnected) {
            isConnected = false;
            return false;
        }
        delay(100);
    }

    isConnected = true;
    if(connectionProbe && !readData().isDataReceived)isConnected = false;

    _ina->setADCMode(SAMPLE_MODE_128);
    return true;    
}

INAData INASensor::readData()
{
    INAData data { false };    

    if(this->isConnected) {
        data.isDataReceived = true;

        data.current = _ina->getCurrent_mA();
        data.voltage = _ina->getBusVoltage_V() + (_ina->getShuntVoltage_mV() / 1000);

        if(isnan(data.voltage) || data.voltage < 0)data.isDataReceived = false;
    }

    return data;
}