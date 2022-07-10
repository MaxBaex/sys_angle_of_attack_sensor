#ifndef TCA9548A_H
#define TCA9548A_H

#include "Arduino.h"
#include <Wire.h>

class TCA9548A {
  public:
    TCA9548A(TwoWire *i2c) : _i2c(i2c){};

    void selectChannel(uint8_t channel);

  private:
    static constexpr int TCA9548A_I2C_ADDRESS = 0x70;

    TwoWire *_i2c;
};

#endif // TCA9548A_H
