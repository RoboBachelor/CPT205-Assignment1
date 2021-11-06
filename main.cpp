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

// Angles of rotation
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

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

void keyboardCallback(unsigned char key, int x, int y) {// keyboard interaction
	if (key == 'm') {
		return;
	}
	if (key == 'M') {
		return;
	}
	// Refresh the window
	glutPostRedisplay();
}

// Respond to arrow keys
void specialKeyCallback(int key, int x, int y){
	if (key == GLUT_KEY_UP)
		xRot -= 5.0f;
	if (key == GLUT_KEY_DOWN)
		xRot += 5.0f;
	if (key == GLUT_KEY_LEFT)
		yRot -= 5.0f;
	if (key == GLUT_KEY_RIGHT)
		yRot += 5.0f;
	xRot = (GLfloat)((const int)xRot % 360);
	yRot = (GLfloat)((const int)yRot % 360);
	// Refresh the window
	glutPostRedisplay();
}

void mouseCallback(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		float cx = (float)x / window_w * width - width / 2;
		float cy = -(float)y / window_h * height + height / 2;
		// Push back the new firework with random location, color and size into the firework list.
		fireworks.push_back(Firework(cx, cy, 500 + getRand(800), getRand(360)));
	}
}

void newFireworkTimerCallback(int value) {
	// Push back the new firework with random location, color and size into the firework list.
	fireworks.push_back(Firework(getRand(width) - width / 2, 60 + getRand(300), 500 + getRand(800), getRand(360)));

	glutTimerFunc(getRand(3000), newFireworkTimerCallback, value + 1);
}

void updateMatrixTimerCallback(int value) {
	if (value < 16) {
		dataRegister = value;
		ballMatrix.setTarget( (float(*)[MATRIX_W]) weight_16numbers[MATRIX_H * value]);
		glutTimerFunc(1500, updateMatrixTimerCallback, value + 1);
	}
	if(value == 16) {
		dataRegister = value;
		ballMatrix.negativeWeights = false;
		ballMatrix.updateRate = 100;
		glutTimerFunc(0, newFireworkTimerCallback, 1);
	}
	if (value >= 16) {
		if (value & 1) {
			ballMatrix.setTarget(weight_15th);
		}
		else {
			ballMatrix.setTarget(weight_xjtlu);
		}
		glutTimerFunc(4000, updateMatrixTimerCallback, value + 1);
	}
}

void newFrameTimerCallback(int value) {

	// Update the next state for all fireworks that are still present.
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

	for (int i = 0; i < 4; ++i) {
		oscilloscopes[i].append((dataRegister >> i) & 1);
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
		glRotatef(xRot, 1.0f, 0.0f, 0.0f);
		glRotatef(yRot, 0.0f, 1.0f, 0.0f);
		ballMatrix.draw();
	glPopMatrix();

	// Draw the waveform in oscilloscopes
	if (dataRegister < 16) {
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


	//打开抗锯齿功能
	//1.开启混合功能
	glEnable(GL_BLEND);

	//2.指定混合因子
	//注意:如果你修改了混合方程式,当你使用混合抗锯齿功能时,请一定要改为默认混合方程式
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//3.开启对点\线\多边形的抗锯齿功能
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);


	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(specialKeyCallback);
	glutKeyboardFunc(keyboardCallback);
	glutMouseFunc(mouseCallback);
	glutDisplayFunc(RenderScene);
	glutTimerFunc(TIME_INC, newFrameTimerCallback, 0);
	glutTimerFunc(0, updateMatrixTimerCallback,0);

	srand(time(NULL));//设置随机数种子，使每次产生的随机序列不同

	glutMainLoop();
	return 0;
}