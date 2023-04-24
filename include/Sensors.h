#pragma once
#include <Models/Weather.h>
#include <Models/PowerLevels.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <PlantowerSensor.h>

#include <BME680Sensor.h>
#include <BMP280Sensor.h>
#include <AHT20Sensor.h>
#include <INASensor.h>

enum class SensorType {ALL, LOW_CURRENT, HIGH_CURRENT};

class Sensors
{
public:
    Sensors();

    void connect(SensorType sensorsType);
    void disconnect(SensorType sensorsType);
    Weather getWeather(uint16_t airQualityMeasurementDurationSeconds);
    PowerLevels getPowerLevels();

private:
    PlantowerSensor* _pms;
    BMP280Sensor* _bmp;
    INASensor* _ina;
    BME680Sensor* _bme;
    AHT20Sensor* _aht;

    EspSoftwareSerial::UART* _pmsSerial;

    void power(SensorType sensorsType, bool turnOn);
    float getDataFromAnalogPin(uint8_t pin);

    bool _isHighCurrentSensorsConnected = false;
    bool _isLowCurrentSensorsConnected = false;
};