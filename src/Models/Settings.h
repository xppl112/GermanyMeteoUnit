#pragma once
#include "Arduino.h"

struct Settings {
  uint16_t sleepDurationSeconds;
  uint8_t measureAirQualityCycle;
  uint16_t airQualityMeasurementDurationSeconds;
};

const Settings DEFAULT_SETTINGS = {
  .sleepDurationSeconds = 1200,
  .measureAirQualityCycle = 3,
  .airQualityMeasurementDurationSeconds = 60
};
