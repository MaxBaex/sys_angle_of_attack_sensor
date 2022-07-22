#include "DataLogging.h"
#include "DataAcquisition.h"
#include <SD.h>
#include <Seeed_Arduino_FreeRTOS.h>
#include <cmath>
#include <cstring>

static constexpr size_t queueLen = 10;
static constexpr size_t msgLen = 80;

DataLogging logging(dataFileHeader);
DataAcquisition acquisition(pdMS_TO_TICKS(1000), &logging);


inline void stop(char *err) {
    while (1) {
        Serial.println(err);
        delay(1000);
    }
}

void setup() {

    Serial.begin(115200);

    delay(2000);

    if (!acquisition.begin()) {
        stop("Data acquisition could not be initialized");
    }

    if (!logging.begin(queueLen, msgLen)) {
        stop("Data logging could not be initialized");
    }

    vTaskStartScheduler();
}

void loop() {}
