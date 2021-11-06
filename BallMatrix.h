#pragma once

#define MATRIX_W 100
#define MATRIX_H 36

#include "Ball.h"
#include <cstdlib>

class BallMatrix {
public:
	Ball balls[MATRIX_W][MATRIX_H];
	float(*target_weight)[MATRIX_W] = NULL;
	bool negativeWeights = true;
	int updateRate = 200;

	BallMatrix();
	void nextState();
	void draw();
	void setTarget(float(*target)[MATRIX_W]);
};
