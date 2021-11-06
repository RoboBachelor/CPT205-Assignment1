#include "BallMatrix.h"
#include "random.h"

static float weightUpdate(float fdb, float set, int updateRate) {
	// Update 20 percent of weights only.
	if (getRand(1000) >= updateRate) {
		return fdb;
	}
	float err = set - fdb;
	return fdb + getRandf(1.f) * err;
}

BallMatrix::BallMatrix() {
	for (int i = 0; i < MATRIX_W; ++i) {
		for (int j = 0; j < MATRIX_H; ++j) {
			balls[i][j].setCenter(-495 + i * 10, -175 + j * 10);
			balls[i][j].setRadius(5.f);
			balls[i][j].weight = getRandf(2.f) - 1.f;
		}
	}
}

void BallMatrix::nextState() {
	// Return if the target weight is point to NULL
	if (!target_weight)
		return;
	// Update the next state for the ball matrix
	for (int i = 0; i < MATRIX_W; ++i) {
		for (int j = 0; j < MATRIX_H; ++j) {
			if(negativeWeights)
				balls[i][j].weight = weightUpdate(balls[i][j].weight, target_weight[MATRIX_H - j - 1][i] * 2.f - 1.f, updateRate);
			else
				balls[i][j].weight = weightUpdate(balls[i][j].weight, target_weight[MATRIX_H - j - 1][i], updateRate);
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

void BallMatrix::setTarget(float(*target)[MATRIX_W]) {
	target_weight = target;
}
