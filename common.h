#pragma once
#define SHADER_EXTENSION ".glsl"
#define GLSL_VERSION "#version 330"
#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1080
#define POINT_SIZE_DELTA 1
#define POINT_SIZE_MIN 1.0
#define POINT_SIZE_MAX 10.0
#define COLOR_DELTA 0.1
#define COLOR_MIN 0.0
#define COLOR_MAX 1.0
#define DEF_CANVAS_COLOR_R 1.0
#define DEF_CANVAS_COLOR_G 1.0
#define DEF_CANVAS_COLOR_B 1.0
#define DEF_CANVAS_COLOR_A 1.0
#define DEF_SIZE 1
#define CANVAS_STACK_LIMIT 10
#define UI_WIDTH 1080
#define UI_HEIGHT 72

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

	Color inverse() { return { 1 - r, 1 - g, 1 - b, a }; }
};


inline Position<double> windowCoordToPixelCoord(Position<double> windowPos) {
	return { (2 * windowPos.xpos / WINDOW_WIDTH - 1), (2 * windowPos.ypos / WINDOW_HEIGHT - 1) };
}


// lower left corner : (0, WINDOW_HEIGHT) ==> (0,0)
inline double adjustYCoord(double coord) { return abs(coord - WINDOW_HEIGHT); }

inline bool isColorDifferent(Color color1, Color color2) { return color1.r != color2.r || color1.g != color2.g || color1.b != color2.b || color1.a != color2.a; }

struct Pixel {
	Position<double> position;
	Color color;
};


struct MouseButton {
	bool right = false;
	bool left = false;
};

struct Mouse {
	Position<double> lastPosition;
	Position<double> currPosition;
	MouseButton pressedButton;
	MouseButton releasedButton;

	bool onlyOneButtonPressed() { return pressedButton.left xor pressedButton.right; }
};
