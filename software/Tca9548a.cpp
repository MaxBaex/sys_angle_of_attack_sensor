#include "Tca9548a.h"

void TCA9548A::selectChannel(uint8_t channel) {
    if (channel > 7) {
        return;
    }

    const uint8_t channel_mask = 1 << channel;

    _i2c->beginTransmission(TCA9548A_I2C_ADDRESS);
    _i2c->write(channel_mask);
    _i2c->endTransmission();
}
