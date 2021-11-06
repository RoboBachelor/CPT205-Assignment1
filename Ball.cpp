#include "main.h"

void Ball::setLuminanceFactor(float factor) {
	luminanceFactor = powf(factor, TIME_INC / 1000.f);
}

void Ball::setCenter(float cx, float cy) {
	this->cx = cx;
	this->cy = cy;
}

void Ball::setRadius(float r) {
	this->radius = r;
}

void Ball::setColor(float h, float s, float v) {
	while (h > 360.f) {
		h -= 360.f;
	}
	while (h < 0.f) {
		h += 360.f;
	}

	this->hsv.h = h;
	this->hsv.s = s;
	this->hsv.v = v;
}

void Ball::setVelocity(float vx, float vy) {
	this->vx = vx;
	this->vy = vy;
}

void Ball::nextState() {
	if (deleted)
		return;

	vx += -2.5f + getRandf(5.f);
	vy += -2.5f + getRandf(5.f);

	cx += vx * TIME_INC / 1000;
	cy += vy * TIME_INC / 1000;

	// Gravity acceleration is 500 pixels / s^2.
	vy -= 500 * TIME_INC / 1000;

	float v_square = vx * vx + vy * vy;
	float resistance = 3.f * v_square * TIME_INC / 1000;
	vx -= resistance * vx / v_square;
	vy -= resistance * vy / v_square;

	// luminance(t) = 0.6 ^ t, where t in seconds
	// Luminance exponential decay function
	alpha *= luminanceFactor;

	if (alpha < 0.01)
		deleted = true;

	return;
}

void Ball::draw() {
	if (deleted)
		return;

	// Convert HSV colorspace to RGB values
	RGB_t rgb = hsv2rgb(this->hsv);

	glBegin(GL_POLYGON);
	glColor4f(rgb.r, rgb.g, rgb.b, alpha);
	// Draw the circle
	for (int i = 0; i <= samplingNumber; ++i) {
		glVertex2f(cx + radius * cosf(2 * PI * i / samplingNumber),
				   cy + radius * sinf(2 * PI * i / samplingNumber));
	}

	glEnd();
}

void Ball::drawForMatrix() {

	// Convert HSV colorspace to RGB values
	RGB_t rgb;
	if (weight > 0) {
		rgb = hsv2rgb(positiveColor);
	}
	else {
		rgb = hsv2rgb(negativeColor);
	}

	glBegin(GL_POLYGON);
	glColor4f(rgb.r, rgb.g, rgb.b, weight * (weight > 0 ? 1 : -1));
	glVertex2f(cx, cy);
	glColor4f(rgb.r, rgb.g, rgb.b, 0.4f * weight * (weight > 0 ? 1 : -1));
	// Draw the circle
	for (int i = 0; i <= samplingNumber; ++i) {
		glVertex2f(cx + radius * cosf(2 * PI * i / samplingNumber),
			cy + radius * sinf(2 * PI * i / samplingNumber));
	}

	glEnd();
}
