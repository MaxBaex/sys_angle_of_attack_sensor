#include "BPS120.h"

BPS120::BPS120(TwoWire *i2c) : _i2c(i2c) {}

bool BPS120::fetchSensorValues() {
    _i2c->requestFrom(BPS120_I2C_ADDRESS, 2);

    const uint8_t msb = _i2c->read();
    const uint8_t lsb = _i2c->read();

    _pressureRaw = 0x3FFF & ((msb << 8) | lsb);
    
    const uint8_t stat = msb >> 6;

    return stat == 0;
}

uint16_t BPS120::getPressureRaw() {
    return _pressureRaw;
}

