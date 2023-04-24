#include <stdint.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h>

struct AHT20Data {
  bool isDataReceived;

  float humidity;
  float temperatureCelsium;
};

class AHT20Sensor
{
public:
  AHT20Sensor(uint8_t i2c_address);

  bool connect(bool connectionProbe = false, bool waitUntilConnected = false);
  AHT20Data readData();

  bool isConnected;

private:
  Adafruit_AHTX0* _aht;
  uint8_t _i2c_address;
};