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
  _pwmRssiAvg = 0;

  for (int i = 0; i < _dataSize; i++) {
    _rssiData[i].pwm = SERVO_PWM_MIN + (rand() % (SERVO_PWM_MAX - SERVO_PWM_MIN));
    _rssiData[i].rssi = ((float)RSSI_SIGNAL_MIN + (float)(rand() % (RSSI_SIGNAL_MAX - RSSI_SIGNAL_MIN))) / (float)RSSI_SIGNAL_MAX;

    _rssiAvg += _rssiData[i].rssi / (float)_dataSize;
    _rssiSqAvg += sq(_rssiData[i].rssi) / (float)_dataSize;
    _pwmRssiAvg += sq(_rssiData[i].rssi) * (float)_rssiData[i].pwm / (float)_dataSize;
  }

  _rssiStdev = sqrt(_rssiSqAvg - sq(_rssiAvg));
  _pwmOptimal = _pwmRssiAvg / _rssiSqAvg;
  _pwmStdev = _rssiStdev * _pwmOptimal / _rssiAvg;

  _sampleCount = 0;
  _samplePos = 0;
  _stepDirection = 1;
}

void BtTracker::updateRssi(int rssi, int pwm) {
  float rssiScaled = (float)rssi / (float)RSSI_SIGNAL_MAX;
  int pos = rand() % _dataSize;

  float prevRssi = _rssiData[pos].rssi;
  uint16_t prevPwm = _rssiData[pos].pwm;

  _rssiData[pos].pwm = (uint16_t)pwm;
  _rssiData[pos].rssi = rssiScaled;

  _sampleCount += 1;

  _rssiAvg += (rssiScaled - prevRssi) / (float)_dataSize;
  _rssiSqAvg += (sq(rssiScaled) - sq(prevRssi)) / (float)_dataSize;
  _rssiStdev = sqrt(_rssiSqAvg - sq(_rssiAvg));

  _pwmRssiAvg += ((sq(rssiScaled) * (float)pwm - sq(prevRssi) * (float)prevPwm)) / (float)_dataSize;
  _pwmOptimal = _pwmRssiAvg / _rssiSqAvg;
  _pwmStdev = _rssiStdev * _pwmOptimal / _rssiAvg;

  if ((float)pwm + SERVO_PWM_STEP >= getPwmMax()) {
    _stepDirection = -1;
  } else if ((float)pwm - SERVO_PWM_STEP <= getPwmMin()) {
    _stepDirection = 1;
  }
}

float BtTracker::getLowRssiFactor() const { 
  return max(0.0f, (float)RSSI_SIGNAL_MIN_THRESHOLD - getRssiAvg()); 
}

float BtTracker::getPwmMin() const { 
  float pwm = getPwmOptimal() - (RSSI_SIGMA_COUNT + getLowRssiFactor()) * getPwmStdev();
  return min((float)SERVO_PWM_MAX, max((float)SERVO_PWM_MIN, pwm)); 
}

float BtTracker::getPwmMax() const { 
  float pwm = getPwmOptimal() + (RSSI_SIGMA_COUNT + getLowRssiFactor()) * getPwmStdev();
  return min((float)SERVO_PWM_MAX, max((float)SERVO_PWM_MIN, pwm)); 
}

int BtTracker::getStep() const {
  float rssiFactor = min(1.0f, getRssiStdev() / getRssiAvg());
  float step = (float)SERVO_PWM_STEP * rssiFactor;
  return _stepDirection * max(SERVO_PWM_MIN_STEP, (int)step);
}