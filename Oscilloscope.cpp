#include "Oscilloscope.h"
#include "Ball.h"
#include <GL/freeglut.h>

Oscilloscope::Oscilloscope() {
	ball.samplingNumber = 30;
	ball.setRadius(10.f);
	ball.setColor(190.f, 1.f, 1.f);
}

void Oscilloscope::append(float value) {
	values[index] = lastValue * (1 - filterCoefficient) + value * filterCoefficient;
	lastValue = values[index++];
	if (index >= DATA_LEN) {
		index = 0;
		isFull = true;
	}
	ball.alpha = 0.2 + 0.6 * lastValue;
}

void Oscilloscope::draw() {
	glPushMatrix();
	glTranslatef(0, 0, -1);
	glBegin(GL_POLYGON);
	glColor4f(1, 1, 1, backgroundAlpha);
	glVertex2f(0, 0);
	glVertex2f(0, verticalScale);
	if (isFull) {
		glVertex2f(horizontalScale * DATA_LEN, verticalScale);
		glVertex2f(horizontalScale * DATA_LEN, 0);
		ball.setCenter(horizontalScale * DATA_LEN - verticalScale * 0.5, verticalScale * 0.5);
	}
	else {
		glVertex2f(horizontalScale * index, verticalScale);
		glVertex2f(horizontalScale * index, 0);
		ball.setCenter(horizontalScale * index - verticalScale * 0.5, verticalScale * 0.5);
	}
	glEnd();
	glTranslatef(0, 0, 0.5);
	ball.draw();
	glPopMatrix();

	glBegin(GL_LINE_STRIP);
	glColor3f(0, 0.5, 1);
	if (isFull) {
		int readIndex = index;
		for (int i = 0; i < DATA_LEN; ++i) {
			glVertex2f(i * horizontalScale, values[readIndex++] * verticalScale);
			if (readIndex >= DATA_LEN) {
				readIndex = 0;
			}
		}
	}
	else {
		for (int i = 0; i < index; ++i) {
			glVertex2f(i * horizontalScale, values[i] * verticalScale);
		}
	}
	glEnd();
}
