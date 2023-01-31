#ifndef BTUTILITIES_H_
#define BTUTILITIES_H_

#include <Arduino.h>

class Sma {
public:
	Sma(int size);

	void reset();
	void add(float input);

  bool ready() const { return _count > size; }

  int size;
  float minVal;
  float maxVal;
  float avg;
  float var;
  float stdev;

private:
  float *_data;
  int _count;
  int _pos;

};

class Ema {
public:
	Ema(float alpha);

	void reset();
	void add(float input);
	float output = 0;

private:
	float _alpha = 0;
	bool _hasInitial = false;
};
#endif
