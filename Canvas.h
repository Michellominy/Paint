#pragma once
#include <queue>
#include <deque>
#include "Shape.h"
#include <stack>
#include <utility>
#include <stdio.h>
#include <windows.h>

extern HANDLE toDrawPixelQueueSem;
extern HANDLE canvasPixelsMutex;
extern HANDLE toDrawPixelQueueMutex;

struct thread_data {
    std::vector<Pixel>* pixelsCanvas; 
    std::queue< std::pair<std::vector<Position<int>>, Color>>* pixelsQueue;
};

void drawPix(std::vector<Pixel>& pixels, int index, Color color);

Position<int> adPo(Position<int> position);

int getIdxWindowPos(Position<int> windowPos);

DWORD WINAPI drawingThread(void* arg);

class Canvas {
public:
    std::vector<Pixel> pixels;
    std::queue< std::pair<std::vector<Position<int>>, Color>> toDrawPixelsQueue;
    std::deque<std::vector<Pixel>> lastCanvas;
    HANDLE drawingThrd;

    Canvas() {
        for (double index_y = 0; index_y < WINDOW_HEIGHT; index_y++)
            for (double index_x = 0; index_x < WINDOW_WIDTH; index_x++)
                pixels.push_back({
                    windowCoordToPixelCoord({index_x, index_y}),
                    {DEF_CANVAS_COLOR_R, DEF_CANVAS_COLOR_G, DEF_CANVAS_COLOR_B, DEF_CANVAS_COLOR_A}
                    });

        struct thread_data* thrd_data = new thread_data;
        thrd_data->pixelsCanvas = &this->pixels;
        thrd_data->pixelsQueue = &this->toDrawPixelsQueue;

        toDrawPixelQueueSem = CreateSemaphore(NULL, 0, TO_DRAW_PIXELS_MAX_SEM_COUNT, NULL);
        canvasPixelsMutex = CreateMutex(NULL, FALSE, NULL);
        toDrawPixelQueueMutex = CreateMutex(NULL, FALSE, NULL);
        drawingThrd = CreateThread(NULL,  0, (LPTHREAD_START_ROUTINE) drawingThread, thrd_data, 0, NULL);
    }

    void save() {
        lastCanvas.push_back(pixels);
        if (lastCanvas.size() > CANVAS_STACK_LIMIT)
            lastCanvas.pop_front();
    }

    void undo(bool eraseLastCanvas = true) {
        if (lastCanvas.empty()) return;

        WaitForSingleObject(canvasPixelsMutex, INFINITE);
        pixels = lastCanvas.back();
        ReleaseMutex(canvasPixelsMutex);

        if (eraseLastCanvas) lastCanvas.pop_back();
    }

    void draw(std::vector<Position<int>> drawingPos, Color color) {
        int currIndex;
        for (Position<int> currPos : drawingPos) {
            currIndex = getIndexOfWindowPos(currPos);
            drawPixel(currIndex, color);
        }
    }

    void fill(Position<int> originPos, Color fillingColor) {
        floodFill(originPos, fillingColor);
    }


    void floodFill(Position<int> originPos, Color newColor) {
        int originPixelIndex = getIndexOfWindowPos(originPos);
        Color oldColor = pixels[originPixelIndex].color;
        if (!isColorDifferent(oldColor, newColor)) return;

        std::stack<Position<int>> positionStack;
        positionStack.push(originPos);

        while (!positionStack.empty()) {
            Position<int> currPos = positionStack.top();
            int currIndex = getIndexOfWindowPos(currPos);
            Color currColor = pixels[currIndex].color;
            positionStack.pop();

            if (isColorDifferent(currColor, oldColor)) continue;

            drawPixel(currIndex, newColor);

            std::vector<Position<int>> neighbours{ {1, 0}, {0, 1}, {-1, 0}, {0, -1} };
            
            for (Position<int> neighbour : neighbours) {
                Position<int> neighbourPos;
                neighbourPos = currPos + neighbour;
                if (Canvas::isInCanvas(neighbourPos)) {
                    positionStack.push(neighbourPos);
                }
            }
        }
    }

    void drawPixel(int index, Color color) {
        pixels[index].color.r = color.r;
        pixels[index].color.g = color.g;
        pixels[index].color.b = color.b;
        pixels[index].color.a = color.a;
    }

    int getIndexOfWindowPos(Position<int> windowPos) {
        Position<int> adjustedpos = adjustPosition(windowPos);
        return adjustedpos.ypos * WINDOW_WIDTH + adjustedpos.xpos;
    }

    Position<int> adjustPosition(Position<int> position) {
        Position<int> adjustedpos = position;

        if (adjustedpos.xpos < 0) adjustedpos.xpos = 0;
        else if (adjustedpos.xpos >= WINDOW_WIDTH) adjustedpos.xpos = WINDOW_WIDTH - 1;

        if (adjustedpos.ypos < 0) adjustedpos.ypos = 0;
        else if (adjustedpos.ypos >= WINDOW_HEIGHT) adjustedpos.ypos = WINDOW_HEIGHT - 1;
        
        return adjustedpos;
    }

    static bool isInCanvas(Position<int> pos) { return pos.xpos < WINDOW_WIDTH&& pos.ypos < WINDOW_HEIGHT - UI_HEIGHT && pos.xpos >= 0 && pos.ypos >= 0; }

    ~Canvas() {
        CloseHandle(this->drawingThrd);
        CloseHandle(canvasPixelsMutex);
        CloseHandle(toDrawPixelQueueSem);
    }
};



