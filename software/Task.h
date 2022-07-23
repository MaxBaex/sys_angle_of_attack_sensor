#ifndef TASKCLASS_H
#define TASKCLASS_H

#include <Seeed_Arduino_FreeRTOS.h>

class Task {
public:
    bool begin(const char *taskName, UBaseType_t priority, size_t stackSize);

    bool isHealthy() const {return _healthy;};

protected:
    virtual void run();

    void setHealthy(bool healthy);

    static inline unsigned long millisRTOS() {
        return xTaskGetTickCount() * 1000 / configTICK_RATE_HZ;
    }

private:
    TaskHandle_t taskHandle;

    static void runStatic(void *pvParameters);

    volatile bool _healthy {true};

};

#endif // TASKCLASS_H
