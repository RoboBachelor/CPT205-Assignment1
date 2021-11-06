#pragma once

#include "main.h"

class Firework {
public:
	int numBalls = 360;
	float cx, cy;
	float maxSpeed = 700;
	float centerHue = 30;
	float timeToLive = 6000;
	std::vector<Ball> balls;

	Firework(float cx, float cy, float maxSpeed, float centerHue);
	bool nextState();
	void draw();
};
