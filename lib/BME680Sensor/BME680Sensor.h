#include <stdint.h>
#include <Wire.h>
#include "Adafruit_BME680.h"

struct BME680Data {
  bool isDataReceived;

  float pressureInHPascals;
  float temperatureCelsium;
  float humidity;
  float gasResistance;
};

class BME680Sensor
{
public:
  BME680Sensor(uint8_t i2c_address);

  bool connect(bool connectionProbe = true, bool waitUntilConnected = false);
  BME680Data readData();

  bool isConnected;

private:
  Adafruit_BME680* _bme;
  uint8_t _i2c_address;
};