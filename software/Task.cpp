#include "Task.h"
#include <Arduino.h>

bool Task::begin(const char *taskName, UBaseType_t priority, size_t stackSize) {
    const auto res = xTaskCreate(runStatic, taskName, stackSize, this,
                tskIDLE_PRIORITY + priority, &taskHandle);
    
    return res == pdPASS;
}

void Task::run() {
    for (;;) {
        Serial.println("Hello my dear friend :)");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void Task::setHealthy(bool healthy) {
        _healthy = healthy;
}

void Task::runStatic(void *pvParameters) {
    Task *instance = (Task*)pvParameters;
    
    instance->run();
}
