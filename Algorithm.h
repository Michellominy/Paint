#pragma once
#include "Point.h"



// https://www.geeksforgeeks.org/bresenhams-line-generation-algorithm/
inline std::vector<Position<int>> bresenhamLine(Position<int> pos1, Position<int> pos2, int dx, int dy, int decide, Point& point, int step, int size)
{
    std::vector<Position<int>> linePosition;
    int pk = 2 * dy - dx;
    for (int i = 0; i <= dx; i += step) {
        if (pk < 0) {
            if (decide == 0) {
                std::vector<Position<int>> neighbourPos = point.getPosition(pos1, size);
                linePosition.insert(linePosition.end(), neighbourPos.begin(), neighbourPos.end());
            }
            else {
                std::vector<Position<int>> neighbourPos = point.getPosition(pos1.reverse(), size);
                linePosition.insert(linePosition.end(), neighbourPos.begin(), neighbourPos.end());
            }
            pk = pk + 2 * dy;
        }
        else {
            if (decide == 0) {
                std::vector<Position<int>> neighbourPos = point.getPosition(pos1, size);
                linePosition.insert(linePosition.end(), neighbourPos.begin(), neighbourPos.end());
            }
            else {
                std::vector<Position<int>> neighbourPos = point.getPosition(pos1.reverse(), size);
                linePosition.insert(linePosition.end(), neighbourPos.begin(), neighbourPos.end());
            }
            pk = pk + 2 * dy - 2 * dx;
            pos1.ypos = pos1.ypos < pos2.ypos ? pos1.ypos + step : pos1.ypos - step;
        }
        pos1.xpos = pos1.xpos < pos2.xpos ? pos1.xpos + step : pos1.xpos - step;
    }
    return linePosition;
}