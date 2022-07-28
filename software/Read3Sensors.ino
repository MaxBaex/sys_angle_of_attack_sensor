#include "DataAcquisition.h"
#include "DataLogger.h"
#include "RTCService.h"
#include "StatusIndication.h"
#include <SD.h>
#include <Seeed_Arduino_FreeRTOS.h>
#include <cmath>
#include <cstring>

DataLogger logging(dataFileHeader);
DataAcquisition acquisition(pdMS_TO_TICKS(100), &logging);
RTCService rtcService(&logging);

const Task *tasklist[] = {(Task *)&logging, (Task *)&acquisition};

StatusIndication statusIndicator(tasklist,
                                 sizeof(tasklist) / sizeof(tasklist[0]));

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

    if (!logging.begin()) {
        stop("Data logging could not be initialized");
    }

    if (!rtcService.begin()) {
        stop("RTC Service could not be initialized");
    }

    statusIndicator.begin();

    vTaskStartScheduler();
}

void loop() {}
