#include <Arduino.h>

#ifndef CONFIG_H
#define CONFIG_H

const unsigned int SERIAL_WARMUP_DELAY_MS = 1000;
const unsigned int SENSOR_WARMUP_DELAY_MS = 1000;
const unsigned int WIFI_CONNECTION_RETRY_ATTEMPTS_COUNT = 2;
const unsigned int WIFI_SEND_DATA_ATTEMPTS_COUNT = 1;

const uint8_t ANALOG_INPUT_PIN = A0;
const uint8_t CONTROL_LED_PIN = D4;

//VCC control
const uint8_t VCC_HIGH_LOAD_PIN = D8;
const uint8_t VCC_LOW_LOAD_PIN = D7;

//SoftwareSerial pins
const uint8_t PMS_TX_PIN = D6;
const uint8_t PMS_RX_PIN = D5;

//Hardware I2C pins used
//const uint8_t I2C_SCL = D1;
//const uint8_t I2C_SDA = D2;
const uint8_t BMP_I2C_ADDR = 0x76;
const uint8_t BME_I2C_ADDR = 0X77;
const uint8_t AHT_I2C_ADDR = 0x38;
const uint8_t INA_I2C_ADDR = 0x40;

struct BackendClientConfig {
    const char* WifiSSID = "BELL763";
    const char* WifiPassword = "";
    const int WifiConnectionTimeout = 20;

    const char* ServerHost = "http://weather.liashko.space";
    const char* ServerApiPostWeatherDataUrl = "/api/weather/PostGermanyReport";
    const int HttpRequestTimeout = 10;
};

#endif