#include <Arduino.h>

#include "config.h"

#include "utilities.h"

#include "BtDisplay.h"
#include "BtButton.h"
#include "BtTracker.h"

#ifdef ARDUINO_AVR_NANO
#include <Servo.h>
#endif

#ifdef ESP_PLATFORM
#include <ESP32_Servo.h>
#endif

BtButton button(BUTTON_PIN, BUTTON_PULLUP);

Servo servo;
BtTracker tracker(RSSI_MAX_SAMPLE_COUNT);
Sma rssiSma(RSSI_TRACKER_STEP_PERIOD_MILLIS / RSSI_SIGNAL_UPDATE_PERIOD_MILLIS);

unsigned long _lastStepTime = 0;
unsigned long _lastUpdateTime = 0;

#ifdef LOG_ENABLED
unsigned long _lastLogRefreshTime = 0;
#endif

#if defined(TFT_ENABLE)
  BtDisplay display;
  unsigned long _lastDisplayRefreshTime = 0;

  void loopDisplay() {
    if (_lastDisplayRefreshTime + TFT_REFRESH_MILLIS < millis()) {
      int pwm = servo.readMicroseconds();

      display.begin(9);
      display.addPoint(0, "sma", rssiSma.avg, rssiSma.minVal, rssiSma.maxVal);
      display.addPoint(1, "rssi avg", tracker.getRssiAvg(), rssiSma.minVal, rssiSma.maxVal);
      display.addPoint(2, "rssi std", tracker.getRssiStdev(), 0, rssiSma.maxVal);
      display.addPoint(3, "pwm", (float)pwm, SERVO_PWM_MIN, SERVO_PWM_MAX);
      display.addPoint(4, "pwm opt", tracker.getPwmOptimal(), SERVO_PWM_MIN, SERVO_PWM_MAX);
      display.addPoint(5, "pwm std", tracker.getPwmStdev(), SERVO_PWM_MIN, SERVO_PWM_MAX);
      display.addPoint(6, "pwm min", tracker.getPwmMin(), SERVO_PWM_MIN, SERVO_PWM_MAX);
      display.addPoint(7, "pwm max", tracker.getPwmMax(), SERVO_PWM_MIN, SERVO_PWM_MAX);
      display.addPoint(8, "rssi max", (float)tracker.getRssiMax(), 0, rssiSma.maxVal);
      display.end();
    }
  }
#endif

void buttonHandler() {
  Serial.println("resetting");
  tracker.reset();
}

void setup() {
  Serial.begin(38400);

#if defined(TFT_ENABLE)
  display.init();
#endif

  button.init();
  button.setHandler(buttonHandler);

  servo.attach(SERVO_PIN, SERVO_PWM_MIN, SERVO_PWM_MAX);
}


void loop() {
  button.update();

  int pwm = 0;
  int rssi = 0;

  if (_lastUpdateTime + RSSI_SIGNAL_UPDATE_PERIOD_MILLIS < millis()) {
    rssi = analogRead(RSSI_PIN);
    rssiSma.add(rssi);

    _lastUpdateTime = millis();
  }

  if (_lastStepTime + RSSI_TRACKER_STEP_PERIOD_MILLIS < millis()) {
    pwm = servo.readMicroseconds();
    tracker.updateRssi(rssiSma.avg, pwm);

    servo.writeMicroseconds(pwm + tracker.getStep());

    _lastStepTime = millis();
  }

#ifdef TFT_ENABLE
  loopDisplay();
#endif

#ifdef LOG_ENABLED
  if (_lastLogRefreshTime + LOG_REFRESH_MILLIS < millis()) {
    if (pwm == 0) {
      pwm = servo.readMicroseconds();
    }
    if (rssi == 0) {
      rssi = analogRead(RSSI_PIN);
    }
    Serial.println("rssi: " + String(rssi) + ", rssi sma: " + String(rssiSma.avg) + "(avg: " + String(tracker.getRssiAvg()) + ", stdev: " + String(tracker.getRssiStdev()) + ", low factor: " + String(tracker.getLowRssiFactor()) + ")" +
                    ", pwm: " + String(tracker.getPwmOptimal()) + " (min: " + String(tracker.getPwmMin()) + ", max: " + String(tracker.getPwmMax()) + ", stdev: " + String(tracker.getPwmStdev()) + ")" +
                    ", step: " + String(tracker.getStep()));

    _lastLogRefreshTime = millis();
  }
#endif
}
