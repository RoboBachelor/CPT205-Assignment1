#include "Oscilloscope.h"
#include "Ball.h"
#include <GL/freeglut.h>

Oscilloscope::Oscilloscope() {
	ball.samplingNumber = 30;
	ball.setRadius(10.f);
	ball.setColor(190.f, 1.f, 1.f);
}

void Oscilloscope::append(float value) {
	values[endIndex] = lastValue * (1 - filterCoefficient) + value * filterCoefficient;
	lastValue = values[endIndex++];
	if (endIndex >= DATA_LEN) {
		endIndex = 0;
	}
	if (len < DATA_LEN) {
		++len;
	}
	if (len == DATA_LEN) {
		beginIndex = endIndex;
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
		glVertex2f(horizontalScale * len, verticalScale);
		glVertex2f(horizontalScale * len, 0);
		glEnd();
		glTranslatef(0, 0, 0.5);
		ball.setCenter(horizontalScale * len - verticalScale * 0.5, verticalScale * 0.5);
		ball.draw();
	glPopMatrix();

	glBegin(GL_LINE_STRIP);
	glColor3f(0, 0.5, 1);

	int readIndex = beginIndex;
	for (int i = 0; i < len; ++i) {
		glVertex2f(i * horizontalScale, values[readIndex] * verticalScale);
		if (++readIndex == DATA_LEN) {
			readIndex = 0;
		}
	}

	glEnd();
}

void Oscilloscope::popHistoryValue() {
	if (++beginIndex >= DATA_LEN) {
		beginIndex -= DATA_LEN;
	}
	--len;
}
