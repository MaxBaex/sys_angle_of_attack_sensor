/**
 * @file RTCService.h
 * @brief Task reading out real time clock, forwarding it to DataLogger
 */
#ifndef RTC_SERVICE_H
#define RTC_SERVICE_H

#include "DataLogger.h"
#include "RTClib.h"
#include "Task.h"

class RTCService : public Task {

  public:
    RTCService(DataLogger *log, size_t intervalMilliSec=5000) : logging(log), _intervalMilliSec(intervalMilliSec){};

    bool begin();

    void run() override;

  private:
    static void buildLogString(char *buffer, size_t bufferSize, DateTime *now,
                               unsigned int timeStamp);

    RTC_PCF8523 rtc;

    size_t _intervalMilliSec;

    DataLogger *logging;
};

#endif // RTC_SERVICE_H
