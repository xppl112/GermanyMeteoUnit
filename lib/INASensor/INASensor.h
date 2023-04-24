#include <stdint.h>
#include <Wire.h>
#include <INA219_WE.h>

struct INAData {
  bool isDataReceived;

  float voltage;
  float current;
};

class INASensor
{
public:
  INASensor(uint8_t i2c_address);

  bool connect(bool connectionProbe = true, bool waitUntilConnected = false);
  INAData readData();

  bool isConnected;

private:
  INA219_WE* _ina;
  uint8_t _i2c_address;
};