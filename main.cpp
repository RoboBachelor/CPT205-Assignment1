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

// Angles of rotation
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

static int window_width = 1280, window_height = 720;

std::list<Firework> fireworks;
extern float weight_xjtlu[][MATRIX_W];
extern float weight_2006[][MATRIX_W];
extern float weight_15th[][MATRIX_W];

float weightUpdate(float fdb, float set) {
	// Update 20 percent of weights only.
	if (getRand(1000) >= 200) {
		return fdb;
	}
	float err = set - fdb;
	return fdb + getRandf(1.f) * err;
}

class BallMatrix {
public:
	Ball balls[MATRIX_W][MATRIX_H];
	float (*target_weight)[MATRIX_W] = weight_2006;

	BallMatrix();
	void nextState();
	void draw();
};

BallMatrix::BallMatrix() {
	for (int i = 0; i < MATRIX_W; ++i) {
		for (int j = 0; j < MATRIX_H; ++j) {
			balls[i][j].setCenter(-495 + i * 10, -175 + j * 10);
			balls[i][j].setRadius(5.f);
			balls[i][j].weight = getRandf(2.f) - 1.f;
		}
	}
}

void BallMatrix::nextState(){
	// Update the next state for the ball matrix
	for (int i = 0; i < MATRIX_W; ++i) {
		for (int j = 0; j < MATRIX_H; ++j) {
			balls[i][j].weight = weightUpdate(balls[i][j].weight, target_weight[MATRIX_H - j - 1][i] * 2.f - 1.f);
		}
	}
}

void BallMatrix::draw() {
	for (int i = 0; i < MATRIX_W; ++i) {
		for (int j = 0; j < MATRIX_H; ++j) {
			balls[i][j].drawForMatrix();
		}
	}
}

BallMatrix ballMatrix;

// Change the view volume and viewport. This is called when the window is resized.
void ChangeSize(int w, int h) {
	if (h == 0) // Avoid division by zero
		h = 1;
	// Set viewport to window dimensions
	glViewport(0, 0, w, h);
	// Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Set viewing volume
	glOrtho(-window_width / 2.f, window_width / 2.f, -window_height / 2.f, window_height / 2.f, -500, 500);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Setting up lighting and material parameters for enhanced rendering effect.
// This topic will be covered later on in the module so please skip this for now.
void SetupRC() {
	// Light parameters and coordinates
	GLfloat whiteLight[] = { 0.45f, 0.45f, 0.45f, 1.0f };
	GLfloat sourceLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
	GLfloat lightPos[] = { -50.f, 25.0f, 250.0f, 0.0f };
	glEnable(GL_DEPTH_TEST); // Hidden surface removal
	glEnable(GL_LIGHTING); // Enable lighting
	// Setup and enable light 0
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, sourceLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sourceLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL); // Enable colour tracking
	// Set material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background
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


void newFireworkTimerCallback(int value) {
	// Push back the new firework with random location, color and size into the firework list.
	fireworks.push_back(Firework(getRand(window_width) - window_width / 2, 60 + getRand(300), 500 + getRand(800), getRand(360)));

	glutTimerFunc(getRand(3000), newFireworkTimerCallback, value + 1);
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

	glutPostRedisplay(); // force OpenGL to redraw the current window
	glutTimerFunc(TIME_INC, newFrameTimerCallback, value + 1);
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
		glVertex3f(-window_width / 2.f, -window_height / 2.f, 0.f);
		glVertex3f(+window_width / 2.f, -window_height / 2.f, 0.f);

		// Darker blue
		glColor3f(0.0f, 0.02f, 0.05f);
		glVertex3f(+window_width / 2.f, +window_height / 2.f, 0.f);
		glVertex3f(-window_width / 2.f, +window_height / 2.f, 0.f);
		glEnd();
	glPopMatrix();

	// Draw all of the exist fireworks in the list
	for (auto it = fireworks.begin(); it != fireworks.end(); ++it) {
		(*it).draw();
	}

	// Draw the matrix of balls
	glPushMatrix();
		glTranslatef(0.0f, -40.0f, +20.0f);
		glRotatef(xRot, 1.0f, 0.0f, 0.0f);
		glRotatef(yRot, 0.0f, 1.0f, 0.0f);
		ballMatrix.draw();
	glPopMatrix();

	// Draw a teapot
	glPushMatrix(); // Save the matrix state and perform rotations
		glTranslatef(-380.0f, 250.0f, +50.0f);
		glRotatef(xRot, 1.0f, 0.0f, 0.0f);
		glRotatef(yRot, 0.0f, 1.0f, 0.0f);
		glColor3f(0.0f, 0.5f, 1.0f);
		// glutWireTeapot(90); // draw a wireframe teapot
	glPopMatrix(); // Restore the matrix state
	glutSwapBuffers();
}
int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("Orthogonal Projection");


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
	glutDisplayFunc(RenderScene);
	glutTimerFunc(TIME_INC, newFrameTimerCallback, 1);
	glutTimerFunc(0, newFireworkTimerCallback, 1);
	//SetupRC();



	srand(time(NULL));//设置随机数种子，使每次产生的随机序列不同

	glutMainLoop();
	return 0;
}