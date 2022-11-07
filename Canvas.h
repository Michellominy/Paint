#pragma once

#include <vector>
#include "common.h"

struct Pixel{
	float position[2];
	float color[3];
};

std::vector<std::vector<int>> neighbours { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };

class Canvas {
public:
	std::vector<Pixel> pixels;

	Canvas() {
		for (int index_y = 0; index_y < WINDOW_HEIGHT; index_y++)
			for (int index_x = 0; index_x < WINDOW_WIDTH; index_x++)
				pixels.push_back({
					{windowCoordToPixelCoord(index_x, xCoord), windowCoordToPixelCoord(index_y, yCoord)},
					{DEF_COLOR_R, DEF_COLOR_G, DEF_COLOR_B}
					});
	}

    void fill(int x, int y, float fillingColor_R, float fillingColor_G, float fillingColor_B) {
        int index = getIndexOfWindowPos(x,y);

        if(pixels[index].color[0] != fillingColor_R && pixels[index].color[1] != fillingColor_G && pixels[index].color[2] != fillingColor_B)
            recursiveFill(x, y, pixels[index].color[0], pixels[index].color[1], pixels[index].color[2], fillingColor_R, fillingColor_G, fillingColor_B);
    }

    void recursiveFill(int x, int y, float OriginColor_R, float OriginColor_G, float OriginColor_B, float fillingColor_R, float fillingColor_G, float fillingColor_B) {
        int pixelIndex = getIndexOfWindowPos(x, y);

        drawPixel(pixelIndex, fillingColor_R, fillingColor_G, fillingColor_B);

        for (std::vector<int> neighbour : neighbours) {
            int neighbourX = x + neighbour[0];
            int neighbourY = y + neighbour[1];
            if (!isOutOfBound(neighbourX, neighbourY)) {
                pixelIndex = getIndexOfWindowPos(neighbourX, neighbourY);

                if (pixels[pixelIndex].color[0] == fillingColor_R && pixels[pixelIndex].color[1] == fillingColor_G && pixels[pixelIndex].color[2] == fillingColor_B)
                    continue;

                if (pixels[pixelIndex].color[0] != OriginColor_R && pixels[pixelIndex].color[1] != OriginColor_G && pixels[pixelIndex].color[2] != OriginColor_B)
                    continue;

                recursiveFill(neighbourX, neighbourY, pixels[pixelIndex].color[0], pixels[pixelIndex].color[1], pixels[pixelIndex].color[2], fillingColor_R, fillingColor_G, fillingColor_B);
            }
        }
    }

	void drawLineBetween(int x1, int y1, int x2, int y2, float color_R, float color_G, float color_B, int size) {
		int dx = abs(x2 - x1);
		int dy = abs(y2 - y1);
        int firstPixelIndex = getIndexOfWindowPos(x1, y1);

        if (dx > dy)
            bresenhamLine(x1, y1, x2, y2, dx, dy, 0, color_R, color_G, color_B, size);
        else
            bresenhamLine(y1, x1, y2, x2, dy, dx, 1, color_R, color_G, color_B, size);

	}
	
    void bresenhamLine(int x1, int y1, int x2, int y2, int dx, int dy, int decide, float color_R, float color_G, float color_B, int size)
    {
        // https://www.geeksforgeeks.org/bresenhams-line-generation-algorithm/
        int pk = 2 * dy - dx;
        for (int i = 0; i <= dx; i++) {
            x1 < x2 ? x1++ : x1--;
            if (pk < 0) {
                if (decide == 0)
                    drawPoint(x1, y1, color_R, color_G, color_B, size);
                else
                    drawPoint(y1, x1, color_R, color_G, color_B, size);
                pk = pk + 2 * dy;
            }
            else {
                y1 < y2 ? y1++ : y1--;
                if (decide == 0)
                    drawPoint(x1, y1, color_R, color_G, color_B, size);
                else
                    drawPoint(y1, x1, color_R, color_G, color_B, size);
                pk = pk + 2 * dy - 2 * dx;
            }
        }
    }

    void drawPoint(int xpos, int ypos, float color_R, float color_G, float color_B, int size) {
        int sizeSquared = size * size;
        for (int y = 0; y <= size; y++)
            for (int x = 0; x <= size; x++)
                if (x * x + y * y <= sizeSquared) {
                    int index_firstQuadrant = getIndexOfWindowPos(xpos+x, ypos+y);
                    int index_secondQuadrant = getIndexOfWindowPos(xpos-x, ypos+y);
                    int index_thirdQuadrant = getIndexOfWindowPos(xpos-x, ypos-y);
                    int index_fourthQuadrant = getIndexOfWindowPos(xpos+x, ypos-y);
                    drawPixel(index_firstQuadrant, color_R, color_G, color_B);
                    drawPixel(index_secondQuadrant, color_R, color_G, color_B);
                    drawPixel(index_thirdQuadrant, color_R, color_G, color_B);
                    drawPixel(index_fourthQuadrant, color_R, color_G, color_B);
                }
    }

    void drawPixel(int index, float color_R, float color_G, float color_B) {
        pixels[index].color[0] = color_R;
        pixels[index].color[1] = color_G;
        pixels[index].color[2] = color_B;
    }

	int getIndexOfWindowPos(int xpos, int ypos) {
        int adjustedYpos = ypos;
        int adjustedXpos = xpos;

        if (xpos < 0) adjustedXpos = 0;
        else if (xpos >= WINDOW_WIDTH) adjustedXpos = WINDOW_WIDTH-1;

        if (ypos < 0) adjustedYpos = 0;
        else if (ypos >= WINDOW_HEIGHT) adjustedYpos = WINDOW_HEIGHT-1;

        return adjustedYpos * WINDOW_WIDTH + adjustedXpos;
    }

    bool isOutOfBound(int x, int y) { return x >= WINDOW_WIDTH || y >= WINDOW_HEIGHT || x < 0 || y < 0; }
};
