#ifndef BTDISPLAY_H_
#define BTDISPLAY_H_

#include "config.h"

#if defined(TFT_ENABLE)

#include <TFT_eSPI.h>
#include <SPI.h>

class BtDisplay {
  public:
    BtDisplay();
    ~BtDisplay();

    void init();

    void begin(uint8_t lineCount);
    void addPoint(int index, const char *name, float value, float rangeMin, float rangeMax);
    void end();

  private:
    TFT_eSPI _tft;
    TFT_eSprite _graph;
};

#endif

#endif
