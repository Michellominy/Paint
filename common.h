#pragma once

#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1080
#define POINT_SIZE_DELTA 1
#define POINT_SIZE_MIN 0
#define POINT_SIZE_MAX 100
#define COLOR_DELTA 0.1
#define COLOR_MIN 0.0
#define COLOR_MAX 1.0
#define DEF_COLOR_R 0.0
#define DEF_COLOR_G 0.0
#define DEF_COLOR_B 0.0

// [0, WINDOW_HEIGHT or WINDOW_WIDTH] to [-1, 1]
enum Coord {xCoord, yCoord};
float windowCoordToPixelCoord(float windowCoord, Coord coordType) {
	return coordType == xCoord ? (2 * windowCoord / WINDOW_WIDTH - 1) : (2 * windowCoord / WINDOW_HEIGHT - 1);
}

double adjustYCoord(double coord) { return abs(coord - WINDOW_HEIGHT); }

struct Position {
	int xpos;
	int ypos;
};

enum Mode {Draw, Select, DrawShape, Fill};

enum Shape{Square, Rectangle, Circle, Triangle};