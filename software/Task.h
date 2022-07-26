#ifndef TASKCLASS_H
#define TASKCLASS_H

#include <Seeed_Arduino_FreeRTOS.h>

class Task {
  public:
    bool begin(const char *taskName, UBaseType_t priority, size_t stackSize);

    bool isHealthy() const { return _healthy; };

    virtual void printStatus() const {};

  protected:
    virtual void run();

    void setHealthy(bool healthy);

  private:
    TaskHandle_t taskHandle;

    static void runStatic(void *pvParameters);

    volatile bool _healthy{true};
};

static inline unsigned long millisRTOS() {
    return xTaskGetTickCount() * 1000 / configTICK_RATE_HZ;
}

#endif // TASKCLASS_H
