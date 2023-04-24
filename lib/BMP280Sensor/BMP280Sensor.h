#include <stdint.h>
#include <Wire.h>
#include "Adafruit_BMP280.h"

struct BMP280Data {
  bool isDataReceived;

  float pressureInHPascals;
  float temperatureCelsium;
};

class BMP280Sensor
{
public:
  BMP280Sensor(uint8_t i2c_address);

  bool connect(bool connectionProbe = true, bool waitUntilConnected = false);
  BMP280Data readData();

  bool isConnected;

private:
  Adafruit_BMP280* _bmp;
  uint8_t _i2c_address;
};