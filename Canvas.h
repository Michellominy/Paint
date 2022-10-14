#pragma once

#include <vector>

//struct Point{};

class Canvas {
public:
	std::vector<float> points;
	int numberOfPoint;

	Canvas() {
		//points.resize(canvasSize);
		numberOfPoint = 0;
	}

	void addPoint(float xpos, float ypos/*, float R, float G, float B, float size*/) {
		points.push_back(xpos);
		points.push_back(ypos);
		numberOfPoint++;
		// TODO: 
		// check vector size and resize if necessary
		// check for repeated value
	}
	
};