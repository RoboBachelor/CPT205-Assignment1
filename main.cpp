/**
 * @file main.cpp
 *
 * @brief Main function of CPT205 assignment.
 *
 * @author Jingyi Wang 1929591
 * 
 * @email Jingyi.Wang1903@student.xjtlu.edu.cn
 */

#include "main.h"
#include "matrixWeights.h"

static int width = 1280, height = 720;
static int window_w, window_h;
static int dataRegister = 0;

std::list<Firework> fireworks;
BallMatrix ballMatrix;
Oscilloscope oscilloscopes[4];

// Change the view volume and viewport. This is called when the window is resized.
void ChangeSize(int w, int h) {
	if (h == 0) // Avoid division by zero
		h = 1;
	window_w = w;
	window_h = h;
	// Set viewport to window dimensions
	glViewport(0, 0, w, h);
	// Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Set viewing volume
	glOrtho(-width / 2.f, width / 2.f, -height / 2.f, height / 2.f, -500, 500);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void mouseCallback(int button, int state, int x, int y) {
	// Detect for left click
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		// Coordinate translation from screen to the world.
		float cx = (float)x / window_w * width - width / 2;
		float cy = -(float)y / window_h * height + height / 2;
		// Push back the new firework with clicked position into the firework list.
		fireworks.push_back(Firework(cx, cy, 500 + getRand(800), getRand(360)));
	}
}

void newFireworkTimerCallback(int value) {
	// Push back the new firework with random location, color and size into the firework list.
	fireworks.push_back(Firework(getRand(width) - width / 2, 60 + getRand(300), 500 + getRand(800), getRand(360)));
	// Register this timer callback with a random period.
	glutTimerFunc(getRand(3000), newFireworkTimerCallback, value + 1);
}

void updateMatrixTimerCallback(int value) {
	// Display from 2006 to 2021 on the matrix.
	if (value < 16) {
		dataRegister = value;
		ballMatrix.setTarget( (float(*)[MATRIX_W]) weight_16numbers[MATRIX_H * value]);
		glutTimerFunc(1500, updateMatrixTimerCallback, value + 1);
	}

	// Register the firework timer.
	// Slowdown the dancing speed of the matrix.
	if(value == 16) {
		ballMatrix.negativeWeights = false;
		ballMatrix.updateRate = 100;
		glutTimerFunc(0, newFireworkTimerCallback, 1);
	}

	// Display two patterns alternately
	// Switch every four seconds
	if (value >= 16) {
		if (value & 1) {
			ballMatrix.setTarget(weight_15th);
		}
		else {
			ballMatrix.setTarget(weight_xjtlu);
		}
		dataRegister = value;
		glutTimerFunc(4000, updateMatrixTimerCallback, value + 1);
	}
}

void newFrameTimerCallback(int value) {

	// Update the next state for all fireworks which are still present.
	// Delete the firework from the list if the current firework is lazy deleted.
	for (auto it = fireworks.begin(); it != fireworks.end();) {
		if (!(*it).nextState()) {
			// Delete current firework and assign the next firework to it.
			it = fireworks.erase(it);
		} else {
			++it;
		}
	}

	// Update the next frame of the ball matrix
	ballMatrix.nextState();

	// Append 4 new values from the data register to the oscilloscopes.
	if (dataRegister < 16) {
		for (int i = 0; i < 4; ++i) {
			oscilloscopes[i].append((dataRegister >> i) & 1);
		}
	}
	else {
		for (int i = 0; i < 4; ++i) {
			oscilloscopes[i].popHistoryValue();
		}
	}

	glutPostRedisplay(); // force OpenGL to redraw the current window
	glutTimerFunc(TIME_INC, newFrameTimerCallback, value + TIME_INC);
}


// Draw scene
void RenderScene(void) {
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Perform the depth test to render multiple objects in the correct order of Z-axis value
	glEnable(GL_DEPTH_TEST); // Hidden surface removal

	// Draw the background in blue-black gradient color
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, -50.0f);
		glBegin(GL_POLYGON);
		// Lighter blue
		glColor3f(0.f, 0.1f, 0.2f);
		glVertex3f(-width / 2.f, -height / 2.f, 0.f);
		glVertex3f(+width / 2.f, -height / 2.f, 0.f);

		// Darker blue
		glColor3f(0.0f, 0.02f, 0.05f);
		glVertex3f(+width / 2.f, +height / 2.f, 0.f);
		glVertex3f(-width / 2.f, +height / 2.f, 0.f);
		glEnd();
	glPopMatrix();

	// Draw all of the exist fireworks in the list
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, -3.0f);
		for (auto it = fireworks.begin(); it != fireworks.end(); ++it) {
			(*it).draw();
		}
	glPopMatrix();

	// Draw the matrix of balls
	glPushMatrix();
		glTranslatef(0.0f, -40.0f, +20.0f);
		ballMatrix.draw();
	glPopMatrix();

	// Draw the waveform in oscilloscopes
	if (dataRegister < 50) {
		glPushMatrix();
		glTranslatef(-width / 2.f, 160.0f, 0.f);
		glLineWidth(1.5);
		for (int i = 0; i < 4; ++i) {
			oscilloscopes[i].draw();
			glTranslatef(0.f, 48.f, 0.f);
		}
		glPopMatrix();
	}

	glutSwapBuffers();
}
int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutCreateWindow("XJTLU 15th Birthday!");

	// Enable anti-aliasing.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);

	// Register the callback functions.
	glutReshapeFunc(ChangeSize);
	glutMouseFunc(mouseCallback);
	glutDisplayFunc(RenderScene);

	// Timer for displaying a new frame in period of 16ms (60Hz)
	glutTimerFunc(TIME_INC, newFrameTimerCallback, 0);

	// Timer for showing the next ball matrix.
	glutTimerFunc(0, updateMatrixTimerCallback,0);

	// Init the random function.
	srand(time(NULL));

	glutMainLoop();
	return 0;
}