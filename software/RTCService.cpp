#include "RTCService.h"
#include <Arduino.h>
#include <string.h>

bool RTCService::begin() {
    if (!rtc.begin()) {
        return false;
    }

    if (!rtc.initialized() || rtc.lostPower()) {
        // TODO: set error state
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    // if rtc was stopped before, start it now, to assure it is running
    rtc.start();

    return Task::begin("RTCService", 1, 256);
}

void RTCService::buildLogString(char *buffer, size_t bufferSize, DateTime *now, unsigned int timeStamp) {

    snprintf(buffer, bufferSize, "% 9u, %04u-%02u-%02u %02u:%02u:%02u\n", timeStamp, now->year(), now->month(), now->day(), now->hour(), now->minute(), now->second());

}

void RTCService::run() {
    while (true) {
        unsigned int timeStamp = millisRTOS();

        taskENTER_CRITICAL();
        DateTime now = rtc.now();
        taskEXIT_CRITICAL();

        char buffer[DataLogger::storeQueueItemLen] = "";

        buildLogString(buffer, sizeof(buffer), &now, timeStamp);

        logging->storeData(buffer);

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
