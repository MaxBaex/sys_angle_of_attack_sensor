#include "Wire.h"

#define TOTALSENSOR_I2C_ADDR 0x28
#define DIFFSENSOR_I2C_ADDR 0x38
#define TCA_I2C_ADDR 0x70


// Create array of raw pressure values
uint16_t pressureRaw[3];
// Create array of raw temperature values
uint16_t temperatureRaw[2];
// Create array of pressure values
double pressure[3];
// Create array of temperature values
float temperature[2];

// Function to control TCA9548A
void tcaselect(uint8_t i) {
  if (i > 7) return;

  Wire.beginTransmission(TCA_I2C_ADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}

// Function to read raw pressure value
// The function returns 0 when the status returned by the sensor differs from 0
uint16_t readRawDiffPressure(TwoWire *i2c = &Wire) {
  // Request for 2-byte data byte(1));
  i2c->requestFrom(DIFFSENSOR_I2C_ADDR, 2);

  // Wait for data to read
  //while (Wire.available() == 0);

  // Read first (MSB) data byte and answer with ACK (continue communication)
  uint8_t msb = i2c->read();
  // Read second (LSB) data byte and answer with NACK (end communication)
  uint8_t lsb = i2c->read();

  // Fit <msb, lsb> into pressureRaw variable
  uint16_t readingRaw = ((uint16_t)msb << 8) | lsb;

  // Extract status
  uint8_t stat = msb >> 6;

  /*
    Serial.print("Status: ");
    Serial.print(stat);
    Serial.print("\t");
  */

  // return 0, if status is not 0
  if (stat != 0) {
    return 0;
  }

  // Remove status bits
  uint16_t pressureRaw = readingRaw & 0x3fff;

  //Return status bits
  uint16_t statusRaw = readingRaw & 0xC000;

  // Return raw pressure
  return pressureRaw;
}

uint16_t readRawDiffTemperature(TwoWire *i2c = &Wire) {
  // Request for 2-byte data byte(1));
  i2c->requestFrom(DIFFSENSOR_I2C_ADDR, 4);

  // Wait for data to read
  //while (Wire.available() == 0);

  // Read first data byte and answer with ACK (continue communication)
  uint8_t fsb = i2c->read();
  // Read second data byte and answer with ACK (continue communication)
  uint8_t ssb = i2c->read();
  // Read thrid data byte and answer with ACK (continue communication)
  uint8_t tsb = i2c->read();
  // Read last data byte and answer with NACK (end communication)
  uint8_t lsb = i2c->read();

  // Fit <msb, lsb> into pressureRaw variable
  uint16_t TempRaw = (((uint16_t)tsb << 8) | lsb) >> 3;

  /*
    Serial.print("Status: ");
    Serial.print(stat);
    Serial.print("\t");
  */

  // Return raw pressure
  return TempRaw;
}

uint16_t readRawTotPressure(TwoWire *i2c = &Wire) {
  // Request for 2-byte data byte(1));
  i2c->requestFrom(TOTALSENSOR_I2C_ADDR, 2);

  // Wait for data to read
  //while (Wire.available() == 0);

  // Read first (MSB) data byte and answer with ACK (continue communication)
  uint8_t msb = i2c->read();
  // Read second (LSB) data byte and answer with NACK (end communication)
  uint8_t lsb = i2c->read();

  // Fit <msb, lsb> into pressureRaw variable
  uint16_t readingRaw = ((uint16_t)msb << 8) | lsb;

  // Extract status
  uint8_t stat = msb >> 6;

  /*
    Serial.print("Status: ");
    Serial.print(stat);
    Serial.print("\t");
  */

  // return 0, if status is not 0
  if (stat != 0) {
    return 0;
  }

  // Remove status bits
  uint16_t pressureRaw = readingRaw & 0x3fff;

  //Return status bits
  uint16_t statusRaw = readingRaw & 0xC000;
  // Return raw pressure
  return pressureRaw;
}

void setup() {
  Wire.begin();

  Serial.begin(115200);

  // Check that devices are present
  // Check at first port
  tcaselect(1);
  Wire.beginTransmission(DIFFSENSOR_I2C_ADDR);
  byte busStatus = Wire.endTransmission();
  if (busStatus != 0x00) {
    Serial.println("I2C Bus Error/Device not Found!");
  }

  // Check at second I2C
  tcaselect(2);
  Wire.beginTransmission(DIFFSENSOR_I2C_ADDR);
  busStatus = Wire.endTransmission();
  if (busStatus != 0x00) {
    Serial.println("I2C Bus Error/Device not Found!");
  }
}

void loop() {
  // Read raw pressure values from sensors at hardware I2C
  tcaselect(1);
  pressureRaw[0] = readRawTotPressure();
  pressureRaw[1] = readRawDiffPressure();
  temperatureRaw[0] = readRawDiffTemperature();
  tcaselect(2);
  pressureRaw[2] = readRawDiffPressure();
  temperatureRaw[1] = readRawDiffTemperature();
  //convert the Raw pressures and temperatures to pressure values in mbar and temperature values in °C
  pressure[0] = ((float)pressureRaw[0]*68.1/16383-33.6)*30/11.8748;
  pressure[1] = ((((float)pressureRaw[1]) * (50) / (16383)) - 24.90)/0.8;
  pressure[2] = ((((float)pressureRaw[2]) * (50) / (16383)) - 24.96)/0.8;

  // temperature formula after kalibration

  temperature[0] = ((float)temperatureRaw[0] * 50 / 2047 - 51.552699387)/1.06687116;
  temperature[1] = ((float)temperatureRaw[1] * 50 / 2047 - 51.552699387)/1.06687116;

  Serial.print("Raw pressure values[mbar]: ");
  Serial.print(pressure[0],4);
  Serial.print(" and ");
  Serial.print(pressure[1],4);
  Serial.print(" and ");
  Serial.print(pressure[2],4);
  Serial.print("  Raw temperature values[°C]: ");
  Serial.print(temperature[0]);
  Serial.print(" and ");
  Serial.print(temperature[1]);
  Serial.println();

  delay(1000);
}
