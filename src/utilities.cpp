#include "utilities.h"

#include <float.h>

Sma::Sma(int size) {
  this->size = size;
  _data = new float[size];
  reset();
}

void Sma::reset() {
  for (int i = 0; i < size; i++) {
    _data[i] = 0.0;
  }

  _count = 0;
  _pos = 0;

  minVal = FLT_MAX; 
  maxVal = -FLT_MIN;
  avg = 0;
  var = 0;
  stdev = 0;
}

void Sma::add(float input) {
  float prevInput = _data[_pos];
  float prevAvg = avg;

  avg = prevAvg + (input - prevInput) / (float)size;
  var += (input - prevInput) * (input - avg + prevInput - prevAvg) / (float)(size - 1);
  stdev = sqrt(var);

  _data[_pos] = input;
  _count += 1;
  _pos = (_pos + 1) % size;

  if (ready()) {
    minVal = min(minVal, input);
    maxVal = max(maxVal, input);
  }
}

Ema::Ema(float alpha) {
	_alpha = alpha;
}

void Ema::reset() {
	_hasInitial = false;
}

void Ema::add(float input) {
	if (_hasInitial) {
		output = _alpha * (input - output) + output;
	} else {
		output = input;
		_hasInitial = true;
	}
}
