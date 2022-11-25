#pragma once

#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1080
#define POINT_SIZE_DELTA 1
#define POINT_SIZE_MIN 1.0
#define POINT_SIZE_MAX 10.0
#define COLOR_DELTA 0.1
#define COLOR_MIN 0.0
#define COLOR_MAX 1.0
#define DEF_COLOR_R 0.0
#define DEF_COLOR_G 0.0
#define DEF_COLOR_B 0.0
#define DEF_COLOR_A 1.0

template <typename T>
struct Position {
	T xpos;
	T ypos;

	operator Position<int>() const { return { int(xpos), int(ypos) }; }

	Position<T> operator+(Position<T> other) { return { xpos + other.xpos, ypos + other.ypos }; }

	Position<T> reverse() { return { ypos, xpos }; }
};

struct Color {
	float r;
	float g;
	float b;
	float a;
};


Position<double> windowCoordToPixelCoord(Position<double> windowPos) {
	return { (2 * windowPos.xpos / WINDOW_WIDTH - 1), (2 * windowPos.ypos / WINDOW_HEIGHT - 1) };
}

double adjustYCoord(double coord) { return abs(coord - WINDOW_HEIGHT); }

bool isColorDifferent(Color color1, Color color2) { return color1.r != color2.r || color1.g != color2.g || color1.b != color2.b || color1.a != color2.a; }


enum Mode {Draw, Select, DrawShape, Fill};
enum Shape{Square, Rectangle, Circle, Triangle};