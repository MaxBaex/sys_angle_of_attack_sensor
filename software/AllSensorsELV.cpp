#include "AllSensorsELV.h"

AllSensorsELVH::AllSensorsELVH(TwoWire *i2c) : _i2c(i2c) {}

bool AllSensorsELVH::fetchPressureValues() {
    _i2c->requestFrom(I2C_ADRESS, 2);

    const uint8_t msb = _i2c->read();
    const uint8_t lsb = _i2c->read();

    _pressureRaw = 0x3FFF & ((msb << 8) | lsb);

    const uint8_t stat = msb >> 6;

    return stat == 0;
}

bool AllSensorsELVH::fetchPressureAndTempValues() {
    _i2c->requestFrom(I2C_ADRESS, 4);

    const uint8_t msb_pressure = _i2c->read();
    const uint8_t lsb_pressure = _i2c->read();

    const uint8_t msb_temp = _i2c->read();
    const uint8_t lsb_temp = _i2c->read();

    _pressureRaw = 0x3FFF & ((msb_pressure << 8) | lsb_pressure);
    _temperatureRaw = (((uint16_t)msb_temp << 8) | lsb_temp) >> 3;

    const uint8_t stat = msb_pressure >> 6;

    return stat == 0;
}
