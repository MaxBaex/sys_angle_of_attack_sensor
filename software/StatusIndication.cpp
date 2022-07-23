#include "StatusIndication.h"
#include <Arduino.h>

#define LED_ON LOW
#define LED_OFF HIGH

bool StatusIndication::begin() {
    pinMode(LED_BUILTIN, OUTPUT);

    Task::begin("StatusIndication", 1, 256);
}

static inline void blinkOnce() {
    digitalWrite(LED_BUILTIN, LED_ON); 
    vTaskDelay(pdMS_TO_TICKS(500));
    digitalWrite(LED_BUILTIN, LED_OFF);
    vTaskDelay(pdMS_TO_TICKS(1000));
}

static inline void ledOff() {
    digitalWrite(LED_BUILTIN, LED_OFF);
    vTaskDelay(1500);
}

void StatusIndication::run() {
    for (;;) {
        bool systemHealthy = true;
        for (int i=0; i<_taskListLen; i++) {
            systemHealthy &= _taskList[i]->isHealthy();
        }

        if (systemHealthy) {
            blinkOnce();
        } else {
            ledOff();
        }
    }
}
