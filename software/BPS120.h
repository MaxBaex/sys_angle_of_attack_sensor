#ifndef BPS120_H
#define BPS120_H

#include "Arduino.h"
#include <Wire.h>

class BPS120 {
  public:
    BPS120(TwoWire *i2c);

    bool fetchSensorValues();

    uint16_t getPressureRaw();

  private:
    static constexpr int BPS120_I2C_ADDRESS = 0x28;

    TwoWire *_i2c;
    uint16_t _pressureRaw{0.0f};
};

#endif // BPS120_H
