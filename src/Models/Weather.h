#pragma once

struct Weather {
    //meteo sensors
    float ahtTemperature;
    int ahtHumidity;
    float bmpTemperature;
    float pressure;
    int raindropLevel;

    //air-quality sensors
    bool isAirQualityMeasured;
    int pm1_0;
    int pm2_5;
    int pm10_0;
    float gasResistance;
    float bmeTemperature;
    int bmeHumidity;
    float bmePressure;
};