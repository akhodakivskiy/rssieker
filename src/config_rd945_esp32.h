#ifndef BTCONFIG_RD945_ESP32_H_
#define BTCONFIG_RD945_ESP32_H_

#define RSSI_PIN 36
//#define RSSI_PIN_2 39

#define RSSI_SIGNAL_MIN 300
#define RSSI_SIGNAL_MAX 2000
#define RSSI_SIGNAL_MIN_THRESHOLD 800
#define RSSI_SIGNAL_MAX_THRESHOLD 1500
#define RSSI_UPDATE_PERIOD_MILLIS 1
#define RSSI_SMA_WINDOW 5
#define RSSI_STEP_PERIOD_MILLIS 5
#define RSSI_DATA_BUFFER_SIZE 320
#define RSSI_SIGMA_COUNT 0.75f

/* *********** display *********** */
//#define TFT_ENABLE - in platform.io
#define TFT_REFRESH_MILLIS 40           // display refresh period
#define TFT_ROTATION 1                  // display orientation, values: 0, 1, 2, 3

/* *********** menu buttons *********** */
#define BUTTON_1_PIN 0                  // arduino pin of the menu button
#define BUTTON_1_PULLUP 1                  
#define BUTTON_2_PIN 35                 // arduino pin of the menu select button
#define BUTTON_2_PULLUP 1                  
#define BUTTON_MILLIS_LONG_DELAY 500    // in milliseconds, time to hold the button to invoke long press handler
#define BUTTON_VALUE_DOWN LOW           // `digitalRead(pin)` return value that corresponds to pressed state

/* *********** servo *********** */
#define SERVO_PWM_MIN 700
#define SERVO_PWM_MAX 2300
#define SERVO_PWM_STEP 20
#define SERVO_PIN 32

#endif
