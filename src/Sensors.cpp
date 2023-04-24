#include "Sensors.h"
#include "config.h"

//#define DEBUG
extern void safeDelay(unsigned int ms);

Sensors::Sensors(){
    _pmsSerial = new EspSoftwareSerial::UART();
    _pms = new PlantowerSensor(_pmsSerial);
    _bme = new BME680Sensor(BME_I2C_ADDR);
    _bmp = new BMP280Sensor(BMP_I2C_ADDR);
    _aht = new AHT20Sensor(AHT_I2C_ADDR);
    _ina = new INASensor(INA_I2C_ADDR);
    
    pinMode(VCC_LOW_LOAD_PIN, OUTPUT);
    pinMode(VCC_HIGH_LOAD_PIN, OUTPUT);
    pinMode(ANALOG_INPUT_PIN, INPUT);
    power(SensorType::ALL, false);
}

void Sensors::connect(SensorType sensorsType){
    //Power up
    if(sensorsType == SensorType::HIGH_CURRENT || sensorsType == SensorType::ALL)power(SensorType::HIGH_CURRENT, true);
    if(sensorsType == SensorType::LOW_CURRENT || sensorsType == SensorType::ALL)power(SensorType::LOW_CURRENT, true);
    safeDelay(SENSOR_WARMUP_DELAY_MS);

    if(sensorsType == SensorType::LOW_CURRENT || sensorsType == SensorType::ALL){
        Wire.begin();delay(100);
        _ina->connect();
        _aht->connect();
        _bmp->connect();

        #ifdef DEBUG
        Serial.print("INA connected: ");Serial.println(_ina->isConnected);
        Serial.print("AHT connected: ");Serial.println(_aht->isConnected);
        Serial.print("BMP connected: ");Serial.println(_bmp->isConnected);
        #endif

        _isLowCurrentSensorsConnected = true;
    }

    if(sensorsType == SensorType::HIGH_CURRENT || sensorsType == SensorType::ALL){
        _pmsSerial->begin(9600, SWSERIAL_8N1, PMS_RX_PIN, PMS_TX_PIN, false);
        safeDelay(SENSOR_WARMUP_DELAY_MS);
        #ifdef DEBUG
        if (!&_pmsSerial) {
            Serial.println("Invalid EspSoftwareSerial initialization"); 
        } 
        #endif

        _bme->connect();
        _pms->connect(false);

        #ifdef DEBUG
        Serial.print("BME connected: ");Serial.println(_bme->isConnected);
        Serial.print("PMS connected: ");Serial.println(_pms->isConnected);
        #endif

        _isHighCurrentSensorsConnected = true;
    }

    safeDelay(SENSOR_WARMUP_DELAY_MS);
}

void Sensors::disconnect(SensorType sensorsType){
    if(sensorsType == SensorType::HIGH_CURRENT || sensorsType == SensorType::ALL){
        _pmsSerial->end();
        delay(100);
        power(SensorType::HIGH_CURRENT, false);
    }
    if(sensorsType == SensorType::LOW_CURRENT || sensorsType == SensorType::ALL){
        power(SensorType::LOW_CURRENT, false);
    }
}

Weather Sensors::getWeather(uint16_t airQualityMeasurementDurationSeconds){
    Weather weatherData;
    weatherData.isAirQualityMeasured = false;

    //Read low-current sensors first while there is no heat
    if(_isLowCurrentSensorsConnected){
        auto ahtData = _aht->readData();
        auto bmpData = _bmp->readData();
        auto rainDropLevel = (1024 - getDataFromAnalogPin(ANALOG_INPUT_PIN)) * 100 / 1024;

        #ifdef DEBUG
        Serial.print("AHT data received: ");Serial.println(ahtData.isDataReceived);
        Serial.print("BMP data received: ");Serial.println(bmpData.isDataReceived);
        Serial.print("Raindrop level: ");Serial.println(rainDropLevel);
        #endif

        weatherData.ahtTemperature = ahtData.isDataReceived ? ahtData.temperatureCelsium : -100;
        weatherData.ahtHumidity = ahtData.isDataReceived ? ahtData.humidity : -1;
        weatherData.bmpTemperature = bmpData.isDataReceived ? bmpData.temperatureCelsium : -100;
        weatherData.pressure = bmpData.isDataReceived ? bmpData.pressureInHPascals : -1;
        weatherData.raindropLevel = rainDropLevel;
    }

    //Read high-current sensors
    if(_isHighCurrentSensorsConnected){
        #ifdef DEBUG
        Serial.print("airQualityMeasurementDurationSeconds: ");Serial.println(airQualityMeasurementDurationSeconds);
        #endif
        auto pmsData = _pms->readDataSyncronioslyAndSleep(airQualityMeasurementDurationSeconds * 1000);
        auto bmeData = _bme->readData();

        #ifdef DEBUG
        Serial.print("BME data received: ");Serial.println(bmeData.isDataReceived);
        Serial.print("PMS data received: ");Serial.println(pmsData.isDataReceived);
        #endif

        weatherData.isAirQualityMeasured = true;
        weatherData.pm1_0 = pmsData.isDataReceived ? pmsData.PM_1_0 : -1;
        weatherData.pm2_5 = pmsData.isDataReceived ? pmsData.PM_2_5 : -1;
        weatherData.pm10_0 = pmsData.isDataReceived ? pmsData.PM_10_0 : -1;
        weatherData.bmeTemperature = bmeData.isDataReceived ? bmeData.temperatureCelsium : -100;
        weatherData.bmeHumidity = bmeData.isDataReceived ? bmeData.humidity : -1;
        weatherData.bmePressure = bmeData.isDataReceived ? bmeData.pressureInHPascals : -1;
        weatherData.gasResistance = bmeData.isDataReceived ? bmeData.gasResistance : -1;
    }

    return weatherData;
}

PowerLevels Sensors::getPowerLevels(){
    PowerLevels data;
    auto inaData = _ina->readData();

    #ifdef DEBUG
    Serial.print("INA data received: ");Serial.print(inaData.isDataReceived);
    Serial.print(" Voltage: ");Serial.println(inaData.voltage);
    #endif

    if(inaData.isDataReceived){
        data.batteryCurrent = inaData.current;
        data.batteryVoltage = inaData.voltage;
    }

    return data;
}

void Sensors::power(SensorType sensorsType, bool turnOn){
    switch (sensorsType)
    {
        case SensorType::LOW_CURRENT:
            digitalWrite(VCC_LOW_LOAD_PIN, turnOn);
            break;
        
        case SensorType::HIGH_CURRENT:
            digitalWrite(VCC_HIGH_LOAD_PIN, turnOn);
            pinMode(PMS_RX_PIN, !turnOn);
            pinMode(PMS_TX_PIN, turnOn);
            break;
        case SensorType::ALL:
            power(SensorType::LOW_CURRENT, turnOn);
            power(SensorType::HIGH_CURRENT, turnOn);
            break;
    }
}

float Sensors::getDataFromAnalogPin(uint8_t pin){
    float Vvalue=0.0;
    for(unsigned int i=0;i<10;i++){
        Vvalue=Vvalue+analogRead(pin); 
        delay(5);
    }
    return Vvalue=(float)Vvalue/10.0;
}