#pragma once
#include <queue>
#include <deque>
#include "ShapeManager.h"

struct Pixel{
	Position<double> position;
	Color color;
};


class Canvas {
public:
	std::vector<Pixel> pixels;
    std::deque<std::vector<Pixel>> lastCanvas;

	Canvas() {
		for (double index_y = 0; index_y < WINDOW_HEIGHT; index_y++)
			for (double index_x = 0; index_x < WINDOW_WIDTH; index_x++)
				pixels.push_back({
                    windowCoordToPixelCoord({index_x, index_y}),
					{DEF_COLOR_R, DEF_COLOR_G, DEF_COLOR_B}
					});
	}

    void save() {
        lastCanvas.push_back(pixels);
        if (lastCanvas.size() > CANVAS_STACK_LIMIT)
            lastCanvas.pop_front();
    }

    void undo() {
        if (lastCanvas.empty()) return;
        pixels = lastCanvas.back();
        lastCanvas.pop_back();
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


    void drawShape(Position<int> pos1, Position<int> pos2, Color color, int size, bool saveOldPixels) {
        static std::vector<Pixel> OldPixelsColor;
        
        if (OldPixelsColor.empty()) {
            // Si c'est vide (premiere fois qu'on entre), alors 
            // 1) Recuperer les positions des pixels que va remplir la shape
            // 2) Sauvegarder les pixels et leurs couleur
            // 3) dessiner la shape avec la size et couleur appropriee
        } else {
            // Sinon (on est deja entree)
            // 1) Redessiner les anciens pixels
            // 2) Recuperer les positions des pixels que va remplir la shape
            if (saveOldPixels) {
                // Si on doit sauvegarder les anciens pixel, alors
                // 1) Sauvegarder les pixels et leurs couleur
            }
            else {
                // Si on ne doit pas savegarder les anciens pixel (derniere fois qu'on entre), alors
                OldPixelsColor.clear();
            }
            // 4) dessiner la shape avec la size et couleur appropriee
        }
    }

	void drawLineBetween(Position<int> pos1, Position<int> pos2, Color color, int size) {
        std::vector<Position<int>> linePos = ShapeManager::getLinePosition(pos1, pos2);
        for (Position<int> currPos : linePos)
            drawPoint(currPos, color, size);
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
        Position<int> adjustedpos = windowPos;

        if (adjustedpos.xpos < 0) adjustedpos.xpos = 0;
        else if (adjustedpos.xpos >= WINDOW_WIDTH) adjustedpos.xpos = WINDOW_WIDTH-1;

        if (adjustedpos.ypos < 0) adjustedpos.ypos = 0;
        else if (adjustedpos.ypos >= WINDOW_HEIGHT) adjustedpos.ypos = WINDOW_HEIGHT-1;

        return adjustedpos.ypos * WINDOW_WIDTH + adjustedpos.xpos;
    }

    bool isOutOfBound(Position<int> pos) { return pos.xpos >= WINDOW_WIDTH || pos.ypos >= WINDOW_HEIGHT || pos.xpos < 0 || pos.ypos < 0; }
};
