#ifndef BTBUTTON_H_
#define BTBUTTON_H_

#include <Arduino.h>

extern "C" {
  typedef void (*handler_function_t)(void);
}

class BtButton {
  public:
    BtButton(uint8_t pin, bool pullup);

    void init();
    void update();

    void setHandler(handler_function_t func);
    void setHandlerLong(handler_function_t func);
  private:
    handler_function_t _handler = NULL;
    handler_function_t _handlerLong = NULL;
    uint8_t _pin;
    uint8_t _pullup;
    long _millisDown;
    bool _isDown;
    bool _isLong;
};

#endif
