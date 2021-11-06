#include "main.h"

int getRand(int N) {
	return rand() % (N + 1);
}

float getRandf(float max) {
	return max * getRand(1000) / 1000;
}
