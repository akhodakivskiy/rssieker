#include "BtTracker.h"

#include <config.h>

BtTracker::BtTracker(int dataSize) {
  _dataSize = dataSize;
  _rssiData = new RssiSample[dataSize];

  reset();
}

BtTracker::~BtTracker() {
}

void BtTracker::reset() {
  _rssiSqAvg = 0;
  _rssiAvg = 0;
  _rssiMax = 0;

  for (int i = 0; i < _dataSize; i++) {
    _rssiData[i].pwm = (uint16_t)SERVO_PWM_MIN + (uint16_t)(rand() % (SERVO_PWM_MAX - SERVO_PWM_MIN));
    _rssiData[i].rssi = (float)(rand() % (RSSI_SIGNAL_MAX - RSSI_SIGNAL_MIN)) / (float)RSSI_SIGNAL_MAX;

    _rssiAvg += _rssiData[i].rssi / (float)_dataSize;
    _rssiSqAvg += sq(_rssiData[i].rssi) / (float)_dataSize;

    if (_rssiMax < _rssiData[i].rssi) {
      _pwmOptimal = _rssiData[i].pwm;
      _rssiMax = _rssiData[i].rssi;
    }
  }

  _rssiStdev = sqrt(_rssiSqAvg - sq(_rssiAvg));
  _pwmStdev = _rssiStdev * _pwmOptimal / _rssiAvg;

  _sampleCount = 0;
  _samplePos = 0;
  _stepDirection = 1;
}

void BtTracker::updateRssi(int rssi, int pwm) {
  float rssiScaled = (float)rssi / (float)RSSI_SIGNAL_MAX;

  float prevRssi = _rssiData[_samplePos].rssi;
  _rssiData[_samplePos].pwm = (uint16_t)pwm;
  _rssiData[_samplePos].rssi = rssiScaled;
  _samplePos = (_samplePos + 1) % _dataSize;
  _sampleCount += 1;

  _rssiAvg += (rssiScaled - prevRssi) / (float)_dataSize;
  _rssiSqAvg += (sq(rssiScaled) - sq(prevRssi)) / (float)_dataSize;
  _rssiStdev = sqrt(_rssiSqAvg - sq(_rssiAvg));

  if (rssiScaled >= _rssiMax) {
    _rssiMax = rssiScaled;
    _pwmOptimal = pwm;
  } else if (prevRssi >= _rssiMax) {
    _rssiMax = 0;
    for (int i = 0; i < _dataSize; i++) {
      if (_rssiData[i].rssi > _rssiMax) {
        _rssiMax = _rssiData[i].rssi;
        _pwmOptimal = _rssiData[i].pwm;
      }
    }
  }
  _pwmStdev = _rssiStdev * _pwmOptimal / _rssiAvg;

  if ((float)pwm + SERVO_PWM_STEP >= getPwmMax()) {
    _stepDirection = -1;
  } else if ((float)pwm - SERVO_PWM_STEP <= getPwmMin()) {
    _stepDirection = 1;
  }
}

float BtTracker::getLowRssiFactor() const { 
  return RSSI_LOW_SIGNAL_FACTOR * max(0.0f, (float)RSSI_SIGNAL_MIN_THRESHOLD - getRssiAvg()) / getRssiAvg(); 
}

float BtTracker::getPwmMin() const { 
  float pwm = getPwmOptimal() - (RSSI_STDEV_SIGMA_FACTOR + getLowRssiFactor()) * getPwmStdev();
  return min((float)SERVO_PWM_MAX, max((float)SERVO_PWM_MIN, pwm)); 
}

float BtTracker::getPwmMax() const { 
  float pwm = getPwmOptimal() + (RSSI_STDEV_SIGMA_FACTOR + getLowRssiFactor()) * getPwmStdev();
  return min((float)SERVO_PWM_MAX, max((float)SERVO_PWM_MIN, pwm)); 
}

int BtTracker::getStep() const {
  float rssiFactor = min(1.0f, getRssiStdev() / getRssiAvg());
  float step = (float)SERVO_PWM_STEP * rssiFactor;
  return _stepDirection * max(SERVO_PWM_MIN_STEP, (int)step);
}