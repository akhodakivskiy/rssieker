#include "BtButton.h"

#include "config.h"

#include <Arduino.h>

BtButton::BtButton(uint8_t pin, bool pullup)  {
  _pin = pin;
  _pullup = pullup;
  _millisDown = 0L;
  _isDown = false;
  _isLong = false;
  _handler = NULL;
  _handlerLong = NULL;
}

void BtButton::init() {
  if (_pullup) {
    pinMode(_pin, INPUT_PULLUP);
  } else {
    pinMode(_pin, INPUT);
  }
}

void BtButton::update() {
  int pinValue = digitalRead(_pin);

  if (pinValue == BUTTON_VALUE_DOWN) {
    if (!_isDown) {
      _isDown = true;
      _millisDown = millis();
    }

    if (_isDown && millis() > _millisDown + BUTTON_MILLIS_LONG_DELAY) {
      if (_handlerLong != NULL) {
        (*_handlerLong)();
      }
      _isDown = false;
      _isLong = true;
    }
  } else if (_isDown) {
    if (!_isLong) {
      if (_handler != NULL) {
        (*_handler)();
      }
    }
    _isDown = false;
    _isLong = false;
  }
}

void BtButton::setHandler(handler_function_t func) {
  _handler = func;
}

void BtButton::setHandlerLong(handler_function_t func) {
  _handlerLong = func;
}
