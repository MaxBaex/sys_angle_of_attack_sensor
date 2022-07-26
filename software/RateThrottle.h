#ifndef RATE_THROTTLE_H
#define RATE_THROTTLE_H

class RateThrottle {
public:

    RateThrottle(unsigned long minPeriod) : _minPeriod(minPeriod){};

    bool waitedLongEnough();

  private:

    const unsigned long _minPeriod;

    unsigned long lastTryTimeStamp;

    bool initialized = false;
};

#endif // RATE_THROTTLE_H
