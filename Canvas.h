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

	Canvas() {
		for (int index_y = 0; index_y < WINDOW_HEIGHT; index_y++)
			for (int index_x = 0; index_x < WINDOW_WIDTH; index_x++)
				points.push_back({
					{windowCoordToPixel(index_x, xCoord), windowCoordToPixel(index_y, yCoord)},
					{DEF_COLOR_R, DEF_COLOR_G, DEF_COLOR_B}
					});
	}

	void drawBetween(int x1, int y1, int x2, int y2) {
		int dx = x2 - x1;
		int dy = y2 - y1;
        int firstPixelIndex = getIndexOfWindowPos(x1, y1);

        if (abs(dx) > abs(dy)) {
            plotPixel(x1, y1, x2, y2, dx, dy);
            glBufferSubData(GL_ARRAY_BUFFER, firstPixelIndex * sizeof(Point), abs(dx) * sizeof(Point), &points[firstPixelIndex]);
        }
        else {
            plotPixel(x1, y1, x2, y2, dx, dy);
            glBufferSubData(GL_ARRAY_BUFFER, firstPixelIndex * sizeof(Point), abs(dy) * sizeof(Point), &points[firstPixelIndex]);
        }

	}
	
    void plotPixel(int x1, int y1, int x2, int y2, int dx, int dy)
    {

        // calculate steps required for generating pixels
        int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

        // calculate increment in x & y for each steps
        float Xinc = dx / (float)steps;
        float Yinc = dy / (float)steps;

        // Put pixel for each step
        float x = x1;
        float y = y1;
        for (int i = 0; i <= steps; i++) {
            points[getIndexOfWindowPos(round(x), round(y))].color[0] = 1.0;
            x += Xinc; // increment in x at each step
            y += Yinc; // increment in y at each step
        }
    }

	int getIndexOfWindowPos(int xpos, int ypos) {return ypos * WINDOW_WIDTH + xpos;}
};