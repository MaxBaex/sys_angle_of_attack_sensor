#ifndef DATA_ACQUISITION_H
#define DATA_ACQUISITION_H

#include "AllSensorsELV.h"
#include "BPS120.h"
#include "Tca9548a.h"
#include "Task.h"
#include "DataLogging.h"
#include <Wire.h>

constexpr char dataFileHeader[] =
    "timeStamp, absolutePressure, diffPressure1, diffPressure2, temp1, temp2\n";
constexpr char dataFileDataTemplate[] =
    "% 10lu % 1.5e, % 1.5e, % 1.5e, % 1.5e, % 1.5e\n";

class DataAcquisition : public Task {

public:

    DataAcquisition(TickType_t targetPeriod, DataLogging *log):
        i2cSwitch(&Wire),
        absPressureSensor(&Wire),
        diffPressureSensor1(&Wire),
        diffPressureSensor2(&Wire),
        logging(log),
        targetTicks(targetPeriod) {};

    bool begin();

    void run() override;

    void printStatus() const override;

protected:

    void measureOnce();

    TCA9548A i2cSwitch;
    BPS120 absPressureSensor;
    AllSensorsELVH diffPressureSensor1;
    AllSensorsELVH diffPressureSensor2;

    DataLogging *logging;

    const TickType_t targetTicks;

private:

    void setTemperature(size_t index, float value);
    float getTemperature(size_t index);
};

#endif //DATA_ACQUISITION_H
