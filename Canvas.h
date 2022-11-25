#pragma once

#include <vector>
#include <queue> 
#include "common.h"

struct Pixel{
	Position<double> position;
	Color color;
};


class Canvas {
public:
	std::vector<Pixel> pixels;

	Canvas() {
		for (double index_y = 0; index_y < WINDOW_HEIGHT; index_y++)
			for (double index_x = 0; index_x < WINDOW_WIDTH; index_x++)
				pixels.push_back({
                    windowCoordToPixelCoord({index_x, index_y}),
					{DEF_COLOR_R, DEF_COLOR_G, DEF_COLOR_B}
					});
	}

    void fill(Position<int> originPos, Color fillingColor) {
        int index = getIndexOfWindowPos(originPos);
        if (isColorDifferent(pixels[index].color, fillingColor))
            iterativeFill(originPos, pixels[index].color, fillingColor);
    }


    void iterativeFill(Position<int> originPos, Color OriginColor, Color fillingColor) {
        std::queue<Position<int>> pixelsQueue;
        Position<int> currPos;
        currPos.xpos = originPos.xpos;
        currPos.ypos = originPos.ypos;
        
        pixelsQueue.push(currPos);

        while (!pixelsQueue.empty()) {

            currPos = pixelsQueue.front();
            int pixelIndex = getIndexOfWindowPos(currPos);
            
            pixelsQueue.pop();

            std::vector<Position<int>> neighbours{ {1, 0}, {0, 1}, {-1, 0}, {0, -1} };

            for (Position<int> neighbour : neighbours) {
                Position<int> neighbourPos;
                neighbourPos.xpos = currPos.xpos + neighbour.xpos;
                neighbourPos.ypos = currPos.ypos + neighbour.ypos;
                if (!isOutOfBound(neighbourPos)) {
                    pixelIndex = getIndexOfWindowPos(neighbourPos);
                    

                    if (isColorDifferent(pixels[pixelIndex].color, OriginColor))
                        continue;
                    drawPixel(pixelIndex, fillingColor);
                    pixelsQueue.push(neighbourPos);
                }
            }

        }
    }

	void drawLineBetween(Position<int> pos1, Position<int> pos2, Color color, int size) {
		int dx = abs(pos2.xpos - pos1.xpos);
		int dy = abs(pos2.ypos - pos1.ypos);
        int firstPixelIndex = getIndexOfWindowPos(pos1);

        if (dx > dy)
            bresenhamLine(pos1, pos2, dx, dy, 0, color, size);
        else
            bresenhamLine({ pos1.ypos, pos1.xpos }, { pos2.ypos, pos2.xpos }, dy, dx, 1, color, size);

	}
	
    void bresenhamLine(Position<int> pos1, Position<int> pos2, int dx, int dy, int decide, Color color, int size)
    {
        // https://www.geeksforgeeks.org/bresenhams-line-generation-algorithm/
        int pk = 2 * dy - dx;
        for (int i = 0; i <= dx; i++) {
            pos1.xpos < pos2.xpos ? pos1.xpos++ : pos1.xpos--;
            if (pk < 0) {
                if (decide == 0)
                    drawPoint(pos1, color, size);
                else
                    drawPoint({ pos1.ypos, pos1.xpos }, color, size);
                pk = pk + 2 * dy;
            }
            else {
                pos1.ypos < pos2.ypos ? pos1.ypos++ : pos1.ypos--;
                if (decide == 0)
                    drawPoint(pos1, color, size);
                else
                    drawPoint({ pos1.ypos, pos1.xpos }, color, size);
                pk = pk + 2 * dy - 2 * dx;
            }
        }
    }

    void drawPoint(Position<int> pointPos, Color color, int size) {
        int sizeSquared = size * size;
        for (int y = 0; y <= size; y++)
            for (int x = 0; x <= size; x++)
                if (x * x + y * y <= sizeSquared) {
                    int index_firstQuadrant = getIndexOfWindowPos({ pointPos.xpos + x, pointPos.ypos + y });
                    int index_secondQuadrant = getIndexOfWindowPos({ pointPos.xpos - x, pointPos.ypos + y });
                    int index_thirdQuadrant = getIndexOfWindowPos({ pointPos.xpos - x, pointPos.ypos - y });
                    int index_fourthQuadrant = getIndexOfWindowPos({ pointPos.xpos + x, pointPos.ypos - y });
                    drawPixel(index_firstQuadrant, color);
                    drawPixel(index_secondQuadrant, color);
                    drawPixel(index_thirdQuadrant, color);
                    drawPixel(index_fourthQuadrant, color);
                }
    }

    void drawPixel(int index, Color color) {
        pixels[index].color.r = color.r;
        pixels[index].color.g = color.g;
        pixels[index].color.b = color.b;
        pixels[index].color.a = color.a;
    }

	int getIndexOfWindowPos(Position<int> windowPos) {
        Position<int> adjustedpos;
        adjustedpos.ypos = windowPos.ypos;
        adjustedpos.xpos = windowPos.xpos;

        if (adjustedpos.xpos < 0) adjustedpos.xpos = 0;
        else if (adjustedpos.xpos >= WINDOW_WIDTH) adjustedpos.xpos = WINDOW_WIDTH-1;

        if (adjustedpos.ypos < 0) adjustedpos.ypos = 0;
        else if (adjustedpos.ypos >= WINDOW_HEIGHT) adjustedpos.ypos = WINDOW_HEIGHT-1;

        return adjustedpos.ypos * WINDOW_WIDTH + adjustedpos.xpos;
    }

    bool isOutOfBound(Position<int> pos) { return pos.xpos >= WINDOW_WIDTH || pos.ypos >= WINDOW_HEIGHT || pos.xpos < 0 || pos.ypos < 0; }
};
