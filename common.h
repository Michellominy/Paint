#pragma once

#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1080
#define POINT_SIZE_DELTA 0.5
#define POINT_SIZE_MIN 1.0
#define POINT_SIZE_MAX 100.0
#define DEF_COLOR_R 0.0
#define DEF_COLOR_G 0.0
#define DEF_COLOR_B 0.0

enum Coord {xCoord, yCoord};
float windowCoordToPixel(float windowCoord, Coord coordType) {
	return coordType == xCoord ? (2 * windowCoord / WINDOW_WIDTH - 1) : (2 * windowCoord / WINDOW_HEIGHT - 1);
}