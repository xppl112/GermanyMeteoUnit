 #include <stdint.h>
 #include "PMS.h"

static const uint32_t PMS_READ_DELAY = 60000;

struct PmsData {
    bool isDataReceived;
    // Atmospheric environment
    uint16_t PM_1_0;
    uint16_t PM_2_5;
    uint16_t PM_10_0;
};

class PlantowerSensor
{
public:
    PlantowerSensor();
    PlantowerSensor(uint8_t rxPin, uint8_t txPin, uint8_t setPin = 0);
    PlantowerSensor(Stream* serial, uint8_t setPin = 0);

    bool connect(bool connectionProbe = true);

    void sleep();
    void wakeUp();

    PmsData readData(uint16_t timeout = 1000);
    PmsData readDataSyncronioslyAndSleep(uint32_t measurementDuration = PMS_READ_DELAY);

    bool isConnected;
    bool isInSleepMode;

private:
    uint8_t _rxPin = 0;
    uint8_t _txPin = 0;
    uint8_t _setPin = 0;
    Stream* _serial;
    PMS* _pms;
};