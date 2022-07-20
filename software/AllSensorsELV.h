#ifndef ALLSENSORS_ELV_H
#define ALLSENSORS_ELV_H

#include "Arduino.h"
#include <Wire.h>

class AllSensorsELVH {
  public:
    AllSensorsELVH(TwoWire *i2c);

    bool isPresent();
    bool fetchPressureValues();
    bool fetchPressureAndTempValues();
    
    inline uint16_t getPressureRaw() { return _pressureRaw; };
    inline uint16_t getTemperatureRaw() { return _temperatureRaw; };

  private:
    static constexpr int I2C_ADRESS = 0x38;

    TwoWire *_i2c;
    uint16_t _pressureRaw{0.0f};
    uint16_t _temperatureRaw{0.0f};
};

#endif // ALLSENSORS_ELV_H
