/**
 * @file DataAcquisition.h
 * @brief Task collecting sensor measurements (pressure and temperature)
 */
#ifndef DATA_ACQUISITION_H
#define DATA_ACQUISITION_H

#include "AllSensorsELV.h"
#include "BPS120.h"
#include "DataLogger.h"
#include "Task.h"
#include "Tca9548a.h"
#include <Wire.h>

constexpr char dataFileHeader[] =
    "timeStamp, absolutePressure, diffPressure1, diffPressure2, temp1, temp2\n";
constexpr char dataFileDataTemplate[] =
    "% 9lu % 1.5e, % 1.5e, % 1.5e, % 1.5e, % 1.5e\n";

class DataAcquisition : public Task {

  public:
    DataAcquisition(DataLogger *log, size_t intervalMilliSec)
        : i2cSwitch(&Wire), absPressureSensor(&Wire),
          diffPressureSensor1(&Wire), diffPressureSensor2(&Wire), logging(log),
          _intervalTicks(pdMS_TO_TICKS(intervalMilliSec))
    {
      
    };

    bool begin();

    void run() override;

    void printStatus() const override;

  protected:
    void measureOnce();

    TCA9548A i2cSwitch;
    BPS120 absPressureSensor;
    AllSensorsELVH diffPressureSensor1;
    AllSensorsELVH diffPressureSensor2;

    DataLogger *logging;

    const TickType_t _intervalTicks;

  private:
    void setTemperature(size_t index, float value);
    float getTemperature(size_t index);
};

#endif // DATA_ACQUISITION_H
