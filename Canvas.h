#pragma once

#include <vector>
#include "common.h"

struct Point{
	float position[2];
	float color[3];
};

class Canvas {
public:
	std::vector<Point> points;
	int numberOfPoint;

	Canvas() {
		numberOfPoint = 0;
		
		for (int index_y = 0; index_y < WINDOW_HEIGHT; index_y++) {
			for (int index_x = 0; index_x < WINDOW_WIDTH; index_x++) {
				points.push_back({
					{windowCoordToPixel(index_x, xCoord), windowCoordToPixel(index_y, yCoord)},
					{DEF_COLOR_R, DEF_COLOR_G, DEF_COLOR_B}
					});
			}
		}
	}

	/*void addPoint(float xpos, float ypos, float R, float G, float B, float size) {
		points.push_back(xpos);
		points.push_back(ypos);
		numberOfPoint++;
		// TODO: 
		// check vector size and resize if necessary
		// check for repeated value
	}*/
	
};