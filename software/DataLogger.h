#ifndef DataLogger_H
#define DataLogger_H

#include "Task.h"
#include "RateThrottle.h"
#include <SD.h>
#include <Seeed_Arduino_FreeRTOS.h>
#include <stddef.h>

#define STR1(x) #x
#define STR(x) STR1(x)

#define LOG_NUM_DIGITS 3

#define LOG_FILE_NAME_LENGTH 11
#define LOG_FILE_NAME_TEMPLATE ("log%0" STR(LOG_NUM_DIGITS) "d.csv")

class LoggingStream {
  public:

    LoggingStream() : restartSDRateThrottle(pdMS_TO_TICKS(2500)) {};

    bool begin(const char *logFileHeader);

    bool isAvailable();

    bool write(const char *logFileHeader, const char *data, size_t dataLength);

  private:
    bool _writeImpl(const char *data, size_t dataLen);

    File logFile;

    static File openLogFile();

    RateThrottle restartSDRateThrottle;
};

class DataLogger : public Task {
  public:
    DataLogger(const char *logFileHeader)
        : _logFileHeader(logFileHeader) {};

    bool begin(UBaseType_t queueLength, UBaseType_t dataItemBytes);

    bool storeData(const void *data, size_t dataSize);

    void run() override;

    void printStatus() const override;

  private:
    LoggingStream logStream;

    QueueHandle_t loggingQueue;

    size_t _queueItemSize;

    const char *_logFileHeader;
};

#endif // DataLogger_H
