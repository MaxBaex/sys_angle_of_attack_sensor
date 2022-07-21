#include "AllSensorsELV.h"
#include "BPS120.h"
#include "Tca9548a.h"
#include "DataLogging.h"
#include <SD.h>
#include <Wire.h>
#include <cstring>

constexpr char logFileHeader[] = "timeStamp, absolutePressure, diffPressure1, diffPressure2, temp1, temp2\n";
constexpr char logFileDataTemplate[] = "% 10d % 1.5e, % 1.5e, % 1.5e, % 1.5e, % 1.5e\n";

TCA9548A i2cSwitch(&Wire);
BPS120 absPressureSensor(&Wire);
AllSensorsELVH diffPressureSensor1(&Wire);
AllSensorsELVH diffPressureSensor2(&Wire);

// Create array of pressure values
double pressure[3];
// Create array of temperature values
float temperature[2];

inline void stop(char *err) {
    while (1) {
        Serial.println(err);
        delay(1000);
    }
}

void setup() {
    Wire.begin();

    Serial.begin(115200);

    // Check that devices are present
    // Check at first port
    i2cSwitch.selectChannel(1);
    if (!diffPressureSensor1.isPresent()) {
        stop("I2C Bus Error/Device not Found!");
    }

    // Check at second I2C
    i2cSwitch.selectChannel(2);
    if (!diffPressureSensor2.isPresent()) {
        stop("I2C Bus Error/Device not Found!");
    }

    delay(5000);

    if (!initializeLogging()) {
        stop("Data logging could not be initialized");
    }

    storeData(logFileHeader, strlen(logFileHeader));
}

void loop() {
    // Read raw pressure values from sensors at hardware I2C
    i2cSwitch.selectChannel(1);
    absPressureSensor.fetchSensorValues();
    diffPressureSensor1.fetchPressureAndTempValues();
    i2cSwitch.selectChannel(2);
    diffPressureSensor2.fetchPressureAndTempValues();

    unsigned long timestamp = millis();

    // convert the Raw pressures and temperatures to pressure values in mbar and
    // temperature values in °C
    pressure[0] =
        ((float)absPressureSensor.getPressureRaw() * 68.1 / 16383 - 33.6) * 30 /
        11.8748;
    pressure[1] =
        ((((float)diffPressureSensor1.getPressureRaw()) * (50) / (16383)) -
         24.90) /
        0.8;
    pressure[2] =
        ((((float)diffPressureSensor1.getPressureRaw()) * (50) / (16383)) -
         24.96) /
        0.8;

    // temperature formula after kalibration

    temperature[0] =
        ((float)diffPressureSensor1.getTemperatureRaw() * 50 / 2047 -
         51.552699387) /
        1.06687116;
    temperature[1] =
        ((float)diffPressureSensor2.getTemperatureRaw() * 50 / 2047 -
         51.552699387) /
        1.06687116;

    char buffer[89] = "";
    snprintf(buffer, 89, logFileDataTemplate, timestamp, pressure[0], pressure[1], pressure[2], temperature[0], temperature[1]);
    storeData(buffer, strlen(buffer));

    Serial.print("Pressure measurements [mbar]: ");
    Serial.print(pressure[0], 4);
    Serial.print(" and ");
    Serial.print(pressure[1], 4);
    Serial.print(" and ");
    Serial.print(pressure[2], 4);
    Serial.println("");
    Serial.print("Temperature measurements [°C]: ");
    Serial.print(temperature[0]);
    Serial.print(" and ");
    Serial.print(temperature[1]);
    Serial.println();

    delay(50);
}
