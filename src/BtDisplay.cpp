#include "BtDisplay.h"

#if defined(TFT_ENABLE)

#include "utilities.h"

const uint16_t _cmap[2] = { TFT_DARKGREY, TFT_GREEN };

BtDisplay::BtDisplay(): _tft(), _graph(&_tft) {
}

BtDisplay::~BtDisplay() {
}

void BtDisplay::init() {
  _tft.init();
  _tft.setRotation(TFT_ROTATION);
  _graph.setColorDepth(8);
  _graph.createSprite(_tft.width(), _tft.height());
  _graph.fillSprite(TFT_BLACK);
}

void BtDisplay::begin(uint8_t lineCount) {
  _graph.scroll(-1, 0);
  _graph.fillRect(0, 0, 100, lineCount * 14, TFT_BLACK);
}

void BtDisplay::addPoint(int index, const char *name, float value, float rangeMin, float rangeMax) {
  int valueScaled = map((int)value, (int)rangeMin, (int)rangeMax, 0, _tft.height());
  uint16_t color = default_4bit_palette[index + 4];
  _graph.drawFastVLine(_tft.width() - 1, _tft.height() - valueScaled, 2, color);

  _graph.setTextColor(color);
  _graph.drawString(String(name) + ": " + String(value), 0, 14 * index);
}

void BtDisplay::end() {

  _graph.pushSprite(0, 0);
}

#endif
