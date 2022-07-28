#include "DataLogger.h"
#include <SPI.h>
#include <string.h>

#define SPI_CS_PIN 3

static inline const File defectiveFile() { return File(); }

File LoggingStream::openLogFile() {
    const int maxLogNum = LOG_NUM_DIGITS * 10 - 1;
    char logFileName[LOG_FILE_NAME_LENGTH];

    for (int i = 0; i <= maxLogNum - 1; i++) {
        snprintf(logFileName, LOG_FILE_NAME_LENGTH, LOG_FILE_NAME_TEMPLATE, i);

        if (!SD.exists(logFileName)) {
            return SD.open(logFileName, FILE_WRITE);
        }
    }

    return defectiveFile();
}

bool LoggingStream::begin(const char *logFileHeader) {

    if (restartSDRateThrottle.waitedLongEnough()) {
        // ensure, that we initialize SD from a cleaned up state
        SD.end();

        if (!SD.begin(SPI_CS_PIN)) {
            return false;
        }
    } else {
        return false;
    }

    logFile = openLogFile();

    if (logFile) {
        const auto dataLen = strlen(logFileHeader);

        return _lowLevelWrite(logFileHeader, dataLen);
    } else {
        return false;
    }
}

bool LoggingStream::write(const char *logFileHeader, const char *data,
                          size_t dataLength) {

    if (!logFile) {
        if (!begin(logFileHeader)) {
            return false;
        }
    }

    if (logFile) {
        return _lowLevelWrite(data, dataLength);
    }

    return false;
}

bool LoggingStream::_lowLevelWrite(const char *data, size_t dataLength) {
    const size_t written = logFile.write(data, dataLength);

    if (written != dataLength) {

        logFile.close();

        logFile = defectiveFile();

        return false;

    } else {
        logFile.flush();

        return true;
    }
}

bool DataLogger::begin() {

    if (_logFileHeader != NULL) {
        logStream.begin(_logFileHeader);
    } else {
        return false;
    }

    loggingQueue = xQueueCreate(queueLen, storeQueueItemLen);

    if (loggingQueue == NULL) {
        Serial.println("Could not create logging queue!");
        return false;
    }

    return Task::begin("DataLogger", 2, 512);
}

void DataLogger::run() {

    char tmpString[itemsWrittenTogehter * storeQueueItemLen] = "";
    size_t cursor = 0;

    for (;;) {
        const auto res = xQueueReceive(loggingQueue, &tmpString[cursor],
                                       pdMS_TO_TICKS(1000));

        if (res == pdTRUE) {
            cursor += strnlen(&tmpString[cursor], storeQueueItemLen);
        } else {
            continue;
        }

        const bool tmpStringFull = cursor + storeQueueItemLen >
                                   itemsWrittenTogehter * storeQueueItemLen;

        if (tmpStringFull) {
            const bool writeSuccesfull =
                logStream.write(_logFileHeader, tmpString, cursor);
            memset(tmpString, '\0', sizeof(tmpString));
            cursor = 0;

            setHealthy(writeSuccesfull);
        }
    }
}

bool DataLogger::storeData(const void *data) {

    return xQueueSend(loggingQueue, data, 0) == pdTRUE;
}

void DataLogger::printStatus() const {
    if (isHealthy()) {
        Serial.println("DataLogger is running :)");
    } else {
        Serial.println("Data logging is not working properly :(");
    }
}
