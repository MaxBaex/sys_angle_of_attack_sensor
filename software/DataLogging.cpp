#include "DataLogging.h"
#include <SPI.h>
#include <string.h>

#define SPI_CS_PIN 3

#define STR_CACHE_SIZE 512

static inline const File defectiveFile() {
    return File();
}

bool DataLogging::begin(UBaseType_t queueLength, UBaseType_t queueItemSize) {

    if (_logFileHeader != NULL) {
        logStream.begin(_logFileHeader);
    } else {
        return false;
    }

    loggingQueue = xQueueCreate( queueLength, queueItemSize);

    if (loggingQueue == NULL) {
        Serial.println("Could not create logging queue!");
        return false;
    }

    _queueItemSize = queueItemSize;

    return Task::begin("DataLogging", 2, 350);
}

void DataLogging::run() {

    char tmpString[STR_CACHE_SIZE] = "";
    size_t cursor = 0;

    for (;;) {
        const auto res = xQueueReceive(loggingQueue, &tmpString[cursor], pdMS_TO_TICKS(1000));

        if (res != pdTRUE) {
            setHealthy(false);
            continue;
        }

        cursor += _queueItemSize;

        const bool wroteLastFittingElement = cursor + _queueItemSize > STR_CACHE_SIZE;

        if (wroteLastFittingElement) {
            const bool writeSuccesfull =  logStream.write(_logFileHeader, tmpString, cursor);
            memset (tmpString,'\0',sizeof(tmpString));
            cursor = 0;

            setHealthy(true);
        }

    }
}

bool DataLogging::storeData(const void *data, size_t dataSize) {
    
    if (dataSize == _queueItemSize) {

        return xQueueSend(loggingQueue, data, 0) == pdTRUE;

    } else {

        return false;
    }
}

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
    SD.end();

    if(!SD.begin(SPI_CS_PIN)) {
        return false;
    }

    logFile = openLogFile();

    if (logFile) {
        const auto dataLen = strlen(logFileHeader);
        
        return _writeImpl(logFileHeader, dataLen);
    } else {
        return false;
    }
}

bool LoggingStream::write(const char *logFileHeader, const char *data, size_t dataLength) {

    if (!logFile) {
        if (!begin(logFileHeader)) {
            return false;
        }
    }

    if (logFile) {

        return _writeImpl(data, dataLength);
    } 

    return false;
}

bool LoggingStream::_writeImpl(const char *data, size_t dataLength) {
    const size_t written = logFile.write(data, dataLength);

    if (written != dataLength) {

        logFile = defectiveFile();

        return false;

    } else {
        logFile.flush();

        return true;
    }

}
