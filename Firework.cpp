/**
 * @file Firework.cpp
 * @brief 
	This is the implementation of the firework objects. The constructor needs
	to specify the position, color, and size of the new fireworks. In each new frame,
	the nextFrame() mathod should be called to change the status of the balls. Then
	draw() mathod needs to be called to draw this firework.

 * @author Jingyi Wang 1929591
 * @email Jingyi.Wang1903@student.xjtlu.edu.cn
 */

#include "Firework.h"

// Specify the position, color (in degree between 0 and 360), and speed of the new fireworks. 
Firework::Firework(float cx, float cy, float maxSpeed, float centerHue) {
	this->cx = cx;
	this->cy = cy;
	this->maxSpeed = maxSpeed;
	this->centerHue = centerHue;

	Ball ball;
	for (int i = 0; i < numBalls; ++i) {
		float speed = maxSpeed * getRandf(1.f);
		ball.setCenter(cx, cy);
		ball.setRadius(0.5f + getRandf(3.f));
		ball.setColor(centerHue - 10 + getRandf(20), 1, 1);
		ball.setVelocity(speed * cosf(i * PI / 180), speed * sinf(i * PI / 180));
		ball.setLuminanceFactor(0.2f + getRand(600) / 1000.f);
		balls.push_back(ball);
	}

}

// Calculate the status in next frame.
// Return false means this firework should be deleted.
bool Firework::nextState() {
	timeToLive -= TIME_INC;
	if (timeToLive < 0) {
		return false;
	}
	for (int i = 0; i < numBalls; ++i) {
		balls[i].nextState();
	}
	return true;
}

// Redraw this firework with updated status.
void Firework::draw() {
	for (int i = 0; i < numBalls; ++i) {
		balls[i].draw();
	}
}
