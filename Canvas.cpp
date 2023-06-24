#include "Canvas.h"

HANDLE toDrawPixelQueueSem;
HANDLE canvasPixelsMutex;
HANDLE toDrawPixelQueueMutex;

void drawPix(std::vector<Pixel>& pixels, int index, Color color) {
    pixels[index].color.r = color.r;
    pixels[index].color.g = color.g;
    pixels[index].color.b = color.b;
    pixels[index].color.a = color.a;
}

Position<int> adPo(Position<int> position) {
    Position<int> adjustedpos = position;

    if (adjustedpos.xpos < 0) adjustedpos.xpos = 0;
    else if (adjustedpos.xpos >= WINDOW_WIDTH) adjustedpos.xpos = WINDOW_WIDTH - 1;

    if (adjustedpos.ypos < 0) adjustedpos.ypos = 0;
    else if (adjustedpos.ypos >= WINDOW_HEIGHT) adjustedpos.ypos = WINDOW_HEIGHT - 1;

    return adjustedpos;
}

int getIdxWindowPos(Position<int> windowPos) {
    Position<int> adjustedpos = adPo(windowPos);
    return adjustedpos.ypos * WINDOW_WIDTH + adjustedpos.xpos;
}


DWORD WINAPI drawingThread(void* arg) {
    struct thread_data* data = (thread_data*)arg;

    std::vector<Pixel>* pixelsCanvas = data->pixelsCanvas;
    std::queue< std::pair<std::vector<Position<int>>, Color>>* pixelsQueue = data->pixelsQueue;

    std::pair<std::vector<Position<int>>, Color> currentPixelsToDraw;
    int currIndex;
    while (1) {
        WaitForSingleObject(toDrawPixelQueueSem, INFINITE);

        WaitForSingleObject(toDrawPixelQueueMutex, INFINITE);
        currentPixelsToDraw = pixelsQueue->front();
        pixelsQueue->pop();
        ReleaseMutex(toDrawPixelQueueMutex);

        WaitForSingleObject(canvasPixelsMutex, INFINITE);

        for (Position<int> currPos : currentPixelsToDraw.first) {
            currIndex = getIdxWindowPos(currPos);
            drawPix(*pixelsCanvas, currIndex, currentPixelsToDraw.second);
        }

        ReleaseMutex(canvasPixelsMutex);
    }
}