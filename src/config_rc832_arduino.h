#ifndef BTCONFIG_RC832_ARDUINO_H_
#define BTCONFIG_RC832_ARDUINO_H_

#define RSSI_PIN A0

#define RSSI_SIGNAL_MIN 100
#define RSSI_SIGNAL_MAX 500
#define RSSI_SIGNAL_MIN_THRESHOLD 200
#define RSSI_UPDATE_PERIOD_MILLIS 10
#define RSSI_SMA_WINDOW 10
#define RSSI_STEP_PERIOD_MILLIS 100
#define RSSI_DATA_BUFFER_SIZE 150
#define RSSI_SIGMA_COUNT 0.75f

/* *********** display *********** */
//#define TFT_ENABLE - in platform.io
#define TFT_REFRESH_MILLIS 40           // display refresh period
#define TFT_ROTATION 1                  // display orientation, values: 0, 1, 2, 3

#define LOG_ENABLED
#define LOG_REFRESH_MILLIS 200

/* *********** menu buttons *********** */
#define BUTTON_PIN 5                  // arduino pin of the menu button
#define BUTTON_PULLUP 1                  
#define BUTTON_MILLIS_LONG_DELAY 500    // in milliseconds, time to hold the button to invoke long press handler
#define BUTTON_VALUE_DOWN LOW           // `digitalRead(pin)` return value that corresponds to pressed state

/* *********** servo *********** */
#define SERVO_STEP_PERIOD_MILLIS 50
#define SERVO_PWM_MIN 1000
#define SERVO_PWM_MAX 2000
#define SERVO_PWM_STEP 30
#define SERVO_PWM_MIN_STEP 10
#define SERVO_PIN 9

#endif
