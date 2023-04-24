//#include <SoftwareSerial.h>
#include <Arduino.h>
#include "PlantowerSensor.h"
#include "PMS.h"
#include "Stream.h"

static PmsData FAILED_DATA = PmsData { false };

PlantowerSensor::PlantowerSensor()
{
    isConnected = false;
}

PlantowerSensor::PlantowerSensor(uint8_t rxPin, uint8_t txPin, uint8_t setPin) : PlantowerSensor()
{
    _rxPin = rxPin;
    _txPin = txPin;
    _setPin = setPin;
}

PlantowerSensor::PlantowerSensor(Stream* serial, uint8_t setPin) : PlantowerSensor()
{
    _serial = serial;
    _setPin = setPin;

    _pms = new PMS(*_serial); 
}

bool PlantowerSensor::connect(bool connectionProbe)
{
    // Switch to passive mode.
    _pms->passiveMode();

    // Default state after sensor power, but undefined after ESP restart e.g. by OTA flash, so we have to manually wake up the sensor for sure.
    // Some logs from bootloader is sent via Serial port to the sensor after power up. This can cause invalid first read or wake up so be patient and wait for next read cycle.
    _pms->wakeUp();    
    delay(1000);
    
    isConnected = true;
    if(connectionProbe && !readData().isDataReceived)isConnected = false;

    //_pms->sleep();
    isInSleepMode = true;

    return isConnected;
}

void PlantowerSensor::sleep()
{
    if(!isConnected) return;

    _pms->sleep();
    isInSleepMode = true;
}

void PlantowerSensor::wakeUp()
{
    if(!isConnected) return;

    _pms->wakeUp();
    isInSleepMode = false;
}

PmsData PlantowerSensor::readData(uint16_t timeout)
{
    if(!isConnected) return FAILED_DATA;

    PMS::DATA data;

    // Clear buffer (removes potentially old data) before read. Some data could have been also sent before switching to passive mode.
    while (_serial->available()) { _serial->read(); }

    _pms->requestRead();

    if (_pms->readUntil(data, timeout))
    {
        return PmsData {
            .isDataReceived = true,
            .PM_1_0 = data.PM_AE_UG_1_0,
            .PM_2_5 = data.PM_AE_UG_2_5,
            .PM_10_0 = data.PM_AE_UG_10_0
        };
    }

    return FAILED_DATA;
}

PmsData PlantowerSensor::readDataSyncronioslyAndSleep(uint32_t measurementDuration)
{
    if(!isConnected) return FAILED_DATA;

    this->wakeUp();

    //safe delay
	for(unsigned int i = 0; i < measurementDuration; i += 1000){
		delay(measurementDuration - i > 1000 ? 1000 : (measurementDuration - i));
        yield();
    }

    PmsData data = this->readData();
    this->sleep();

    return data;
}