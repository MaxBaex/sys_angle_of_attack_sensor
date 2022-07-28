#ifndef RTC_SERVICE_H
#define RTC_SERVICE_H

#include "DataLogger.h"
#include "RTClib.h"

class RTCService : public Task {

  public:
    RTCService(DataLogger *log) : logging(log){};

    bool begin();

    void run() override;

  private:
    static void buildLogString(char *buffer, size_t bufferSize, DateTime *now,
                               unsigned int timeStamp);

    RTC_PCF8523 rtc;

    DataLogger *logging;
};

#endif // RTC_SERVICE_H
