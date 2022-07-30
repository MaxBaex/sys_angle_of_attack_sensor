#include "RateThrottle.h"
#include "Task.h"

bool RateThrottle::waitedLongEnough() {

    if (initialized) {

        const auto waitedMillis = millisRTOS() - lastTryTimeStamp;

        const bool waitedEnough = waitedMillis >= _minPeriod;

        if (waitedEnough) {
            lastTryTimeStamp = millisRTOS();
        }

        return waitedEnough;

    } else {
        lastTryTimeStamp = millisRTOS();

        initialized = true;

        return true;
    }
}
