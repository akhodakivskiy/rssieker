#ifndef BTTRACKER_H_
#define BTTRACKER_H_

#include "config.h"
#include "utilities.h"

#include <Arduino.h>

struct RssiSample {
  uint16_t pwm;
  float rssi;
};

class BtTracker {
  public:
    BtTracker(int dataSize);
    ~BtTracker();

    void reset();

    void updateRssi(int rssi, int pwm);

    float getRssiMax() const { return _rssiMax * (float)RSSI_SIGNAL_MAX; }
    float getRssiAvg() const { return _rssiAvg * (float)RSSI_SIGNAL_MAX; }
    float getRssiStdev() const { return _rssiStdev * (float)RSSI_SIGNAL_MAX; }

    float getPwmOptimal() const { return _pwmOptimal; }
    float getPwmStdev() const { return _pwmStdev; }

    float getLowRssiFactor() const;
    float getPwmMin() const;
    float getPwmMax() const;
    int getStep() const;
  private:
    float _rssiSqAvg;
    float _rssiAvg;
    float _rssiStdev;
    float _rssiMax;

    float _pwmOptimal;
    float _pwmStdev;

    RssiSample *_rssiData;
    int _dataSize = 0;
    int _samplePos = 0;
    int _sampleCount = 0;
    int _stepDirection = 0;
};

#endif