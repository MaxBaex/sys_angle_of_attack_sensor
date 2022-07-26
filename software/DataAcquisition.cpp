#include "DataAcquisition.h"
#include <Arduino.h>

bool DataAcquisition::begin() {
    Wire.begin();

    // Check that devices are present
    // Check at first port
    i2cSwitch.selectChannel(1);
    if (!diffPressureSensor1.isPresent()) {
        return false;
    }

    // Check at second I2C
    i2cSwitch.selectChannel(2);
    if (!diffPressureSensor2.isPresent()) {
        return false;
    }

    return Task::begin("DataAcquisition", 3, 256);
}

void DataAcquisition::run() {

    while (true) {
        const TickType_t ticksMeasurementStart = xTaskGetTickCount();
        measureOnce();
        const TickType_t measurementTicks = xTaskGetTickCount() - ticksMeasurementStart;

        if (targetTicks > measurementTicks) {
            vTaskDelay(targetTicks - measurementTicks);
        }
    }
}

void DataAcquisition::measureOnce() {

    unsigned int timeStamp = millisRTOS();
    // Create array of pressure values
    double pressure[3];
    // Create array of temperature values
    float temperature[2];

    // Read raw pressure values from sensors at hardware I2C
    i2cSwitch.selectChannel(1);
    absPressureSensor.fetchSensorValues();
    diffPressureSensor1.fetchPressureAndTempValues();
    i2cSwitch.selectChannel(2);
    diffPressureSensor2.fetchPressureAndTempValues();

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

    // temperature formula after calibration

    temperature[0] =
        ((float)diffPressureSensor1.getTemperatureRaw() * 50 / 2047 -
         51.552699387) /
        1.06687116;
    temperature[1] =
        ((float)diffPressureSensor2.getTemperatureRaw() * 50 / 2047 -
         51.552699387) /
        1.06687116;

    char buffer[128] = "";
    snprintf(buffer, 128, dataFileDataTemplate, timeStamp, pressure[0],
             pressure[1], pressure[2], temperature[0], temperature[1]);
    logging->storeData(buffer, strlen(buffer));
}

void DataAcquisition::printStatus() const {
    Serial.println("Data acquisition is running :)");
}
