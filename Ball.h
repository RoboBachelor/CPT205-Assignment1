#pragma once

#include "color.h"

class Ball {
public:
	float cx = 0, cy = 0;	// Coordinate of center
	float radius = 4.5;		// Radius
	float vx = 0, vy = 0;	// Velocity, pixels per second.
	HSV_t hsv = { 0 };		// Color in HSV color space
	float luminanceFactor = 1.f;	// Luminance exponential decay factor
	float alpha = 1.f;				// Luminance
	bool deleted = false;	// Lazy delete flag

	// Color for matrix of balls. v[ORANGE]v						v[PURPLE]v
	HSV_t positiveColor = { 35.f, 1.f, 1.f }, negativeColor = { 285.f, 0.5f, 1.f };
	// Weight value for matrix of balls.
	float weight = 0.f;

	void setCenter(float cx, float cy);
	void setRadius(float radius);
	void setColor(float h, float s, float v);
	void setVelocity(float vx, float vy);
	void setLuminanceFactor(float factor);
	void nextState(void);
	void draw(void);
	void drawForMatrix(void);

};
