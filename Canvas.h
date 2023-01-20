#pragma once

#include <vector>
#include <queue> 
#include "common.h"



class Canvas {
public:
	std::vector<Pixel> pixels;

	Canvas() {
		for (double index_y = 0; index_y < WINDOW_HEIGHT; index_y++)
			for (double index_x = 0; index_x < WINDOW_WIDTH; index_x++)
				pixels.push_back({
                    windowCoordToPixelCoord({index_x, index_y}),
					{DEF_CANVAS_COLOR_R, DEF_CANVAS_COLOR_G, DEF_CANVAS_COLOR_B, DEF_CANVAS_COLOR_A}
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
                neighbourPos = currPos + neighbour;
                if (Canvas::isInCanvas(neighbourPos)) {
                    pixelIndex = getIndexOfWindowPos(neighbourPos);

                    if (isColorDifferent(pixels[pixelIndex].color, OriginColor))
                        continue;

                    drawPixel(pixelIndex, fillingColor);
                    pixelsQueue.push(neighbourPos);
                }
            }

        }
    }

    void draw(Position<int> pos1, Position<int> pos2, Color color, int size, Brush& brush) {
        std::vector<Position<int>> brushPos = brush.getLine(pos1, pos2, size);
        int currIndex;
        for (Position<int> currPos : brushPos) {
            currIndex = getIndexOfWindowPos(currPos);
            drawPixel(currIndex, color);
        }
    }

    void drawShape(Position<int> pos1, Position<int> pos2, Color color, int size, Shape &shape) {
        std::vector<Position<int>> shapePos = shape.getPosition(pos1, pos2, size);
        int currIndex;
        for (Position<int> currPos : shapePos) {
            currIndex = getIndexOfWindowPos(currPos);
            drawPixel(currIndex, color);
        }
    }


    void drawPoint(Position<int> pos, Color color, int size, Brush& brush) {
        std::vector<Position<int>> pointPos = brush.point->getPosition(pos, size);
        int currIndex;
        for (Position<int> currPos : pointPos) {
            currIndex = getIndexOfWindowPos(currPos);
            drawPixel(currIndex, color);
        }
    }

    void drawPixel(int index, Color color) {
        pixels[index].color.r = color.r;
        pixels[index].color.g = color.g;
        pixels[index].color.b = color.b;
        pixels[index].color.a = color.a;
    }

	int getIndexOfWindowPos(Position<int> windowPos) {
        Position<int> adjustedpos = windowPos;

        if (adjustedpos.xpos < 0) adjustedpos.xpos = 0;
        else if (adjustedpos.xpos >= WINDOW_WIDTH) adjustedpos.xpos = WINDOW_WIDTH-1;

        if (adjustedpos.ypos < 0) adjustedpos.ypos = 0;
        else if (adjustedpos.ypos >= WINDOW_HEIGHT) adjustedpos.ypos = WINDOW_HEIGHT-1;

        return adjustedpos.ypos * WINDOW_WIDTH + adjustedpos.xpos;
    }

    static bool isInCanvas(Position<int> pos) { return pos.xpos < WINDOW_WIDTH && pos.ypos < WINDOW_HEIGHT-UI_HEIGHT && pos.xpos >= 0 && pos.ypos >= 0; }
};
