#pragma once
#include <vector>
#include "common.h"

std::vector<Position<int>> bresenhamLine(Position<int> pos1, Position<int> pos2, int dx, int dy, int decide);

class Shape {
public:
    virtual std::vector<Position<int>> getPosition(Position<int> pos1, Position<int> pos2) { return {}; };
};

class Line : public Shape {
public:
    std::vector<Position<int>> getPosition(Position<int> pos1, Position<int> pos2) {
        int dx = abs(pos2.xpos - pos1.xpos);
        int dy = abs(pos2.ypos - pos1.ypos);
        if (dx > dy)
            return bresenhamLine(pos1, pos2, dx, dy, 0);
        else
            return bresenhamLine(pos1.reverse(), pos2.reverse(), dy, dx, 1);
    }
};

class Rectangle : public Shape {
public:
    std::vector<Position<int>> getPosition(Position<int> pos1, Position<int> pos2) {
        std::vector<Position<int>> rectanglePosition;
        std::vector<Position<int>> newPos;
        Line line;
        Position<int> pos3 = { pos2.xpos, pos1.ypos };
        Position<int> pos4 = {pos1.xpos, pos2.ypos};

        newPos = line.getPosition(pos1, pos3);
        rectanglePosition.insert(rectanglePosition.end(), newPos.begin(), newPos.end());
        newPos = line.getPosition(pos1, pos4);
        rectanglePosition.insert(rectanglePosition.end(), newPos.begin(), newPos.end());
        newPos = line.getPosition(pos2, pos3);
        rectanglePosition.insert(rectanglePosition.end(), newPos.begin(), newPos.end());
        newPos = line.getPosition(pos2, pos4);
        rectanglePosition.insert(rectanglePosition.end(), newPos.begin(), newPos.end());

        return rectanglePosition;
    }
};



// https://www.geeksforgeeks.org/bresenhams-line-generation-algorithm/
std::vector<Position<int>> bresenhamLine(Position<int> pos1, Position<int> pos2, int dx, int dy, int decide)
{
    std::vector<Position<int>> linePosition;
    int pk = 2 * dy - dx;
    for (int i = 0; i <= dx; i++) {
        pos1.xpos < pos2.xpos ? pos1.xpos++ : pos1.xpos--;
        if (pk < 0) {
            if (decide == 0)
                linePosition.push_back(pos1);
            else
                linePosition.push_back(pos1.reverse());
            pk = pk + 2 * dy;
        }
        else {
            pos1.ypos < pos2.ypos ? pos1.ypos++ : pos1.ypos--;
            if (decide == 0)
                linePosition.push_back(pos1);
            else
                linePosition.push_back(pos1.reverse());
            pk = pk + 2 * dy - 2 * dx;
        }
    }
    return linePosition;
}