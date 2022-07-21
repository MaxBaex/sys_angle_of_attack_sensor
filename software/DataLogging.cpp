#include "DataLogging.h"
#include <SD.h>
#include <SPI.h>

#define STR1(x) #x
#define STR(x) STR1(x)

#define SPI_CS_PIN 3
#define LOG_NUM_DIGITS 3

#define LOG_FILE_NAME_TEMPLATE ("log%0" STR(LOG_NUM_DIGITS) "d.csv")

static int logNum = 0;

int determineLogNum() {
    const int maxLogNum = LOG_NUM_DIGITS * 10 - 1;

    for (int i = 0; i <= maxLogNum - 1; i++) {
        char logFileName[11] = "";
        snprintf(logFileName, 11, LOG_FILE_NAME_TEMPLATE, i);

        if (!SD.exists(logFileName)) {
            return i;
        }
    }

    return 0;
}

bool initializeLogging() {

    if (!SD.begin(SPI_CS_PIN)) {
        Serial.println("SD card could not be initialized!");
        return false;
    }

    logNum = determineLogNum();

    const bool hasLogNum = logNum >= 0;

    return hasLogNum;
}

void storeData(const void *data, size_t dataSize) {
    char logFileName[11] = "";
    snprintf(logFileName, 11, LOG_FILE_NAME_TEMPLATE, logNum);

    File dataFile = SD.open(logFileName, FILE_WRITE);

    if (dataFile) {
        dataFile.write((const char *)data, dataSize);

        dataFile.close();
    }
}
