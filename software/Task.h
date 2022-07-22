#ifndef TASKCLASS_H
#define TASKCLASS_H

#include <Seeed_Arduino_FreeRTOS.h>

class Task {
public:
    bool begin(const char *taskName, UBaseType_t priority, size_t stackSize);

protected:
    virtual void run();

private:
    TaskHandle_t taskHandle;

    static void runStatic(void *pvParameters);

protected:
    static inline unsigned long millisRTOS() {
        return xTaskGetTickCount() * 1000 / configTICK_RATE_HZ;
    }
};

#endif // TASKCLASS_H
