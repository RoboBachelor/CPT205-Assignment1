#pragma once

#define DATA_LEN 500

#include "Ball.h"

class Oscilloscope {
public:
	float values[DATA_LEN] = { 0 };
	int index = 0;
	bool isFull = false;
	float horizontalScale = 2.f;
	float verticalScale = 36.f;
	float filterCoefficient = 0.9;
	float lastValue = 0;
	float backgroundAlpha = 0.05;

	Ball ball;

	Oscilloscope();
	void append(float value);
	void draw();
};
