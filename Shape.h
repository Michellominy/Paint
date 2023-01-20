#pragma once
#include <vector>
#include "Brushes.h"




class Shape {
public:
    std::string name;

    virtual std::vector<Position<int>> getPosition(Position<int> pos1, Position<int> pos2, int size) = 0;
};

class Line : public Shape {
public:
    Line() {
        this->name = "Line";
    }

    std::vector<Position<int>> getPosition(Position<int> pos1, Position<int> pos2, int size) {
        int dx = abs(pos2.xpos - pos1.xpos);
        int dy = abs(pos2.ypos - pos1.ypos);
        CircleP point = CircleP();
        if (dx > dy)
            return bresenhamLine(pos1, pos2, dx, dy, 0, point, 1, size);
        else
            return bresenhamLine(pos1.reverse(), pos2.reverse(), dy, dx, 1, point, 1, size);
    }
};

class Rectangle : public Shape {
public:
    Rectangle() {
        this->name = "Rectangle";
    }

    std::vector<Position<int>> getPosition(Position<int> pos1, Position<int> pos2, int size) {
        std::vector<Position<int>> rectanglePosition;
        std::vector<Position<int>> newPos;
        Line line;
        Position<int> pos3 = { pos2.xpos, pos1.ypos };
        Position<int> pos4 = { pos1.xpos, pos2.ypos };

        newPos = line.getPosition(pos1, pos3, size);
        rectanglePosition.insert(rectanglePosition.end(), newPos.begin(), newPos.end());
        newPos = line.getPosition(pos1, pos4, size);
        rectanglePosition.insert(rectanglePosition.end(), newPos.begin(), newPos.end());
        newPos = line.getPosition(pos2, pos3, size);
        rectanglePosition.insert(rectanglePosition.end(), newPos.begin(), newPos.end());
        newPos = line.getPosition(pos2, pos4, size);
        rectanglePosition.insert(rectanglePosition.end(), newPos.begin(), newPos.end());

        return rectanglePosition;
    }
};


class Triangle : public Shape {
public:
    Triangle() {
        this->name = "Triangle";
    }

    std::vector<Position<int>> getPosition(Position<int> pos1, Position<int> pos2, int size) {
        /*std::vector<Position<int>> trianglePosition;
        std::vector<Position<int>> newPos;
        Line line;
        int middleXpos = abs(pos1.xpos - pos2.xpos) / 2;
        Position<int> upperCorner = { middleXpos, pos1.ypos };
        Position<int> leftCorner = { pos1.xpos, pos2.ypos };
        Position<int> rightCorner = pos2;

        newPos = brush.getLine(upperCorner, leftCorner);
        trianglePosition.insert(trianglePosition.end(), newPos.begin(), newPos.end());
        newPos = brush.getLine(leftCorner, rightCorner);
        trianglePosition.insert(trianglePosition.end(), newPos.begin(), newPos.end());
        newPos = brush.getLine(rightCorner, upperCorner);
        trianglePosition.insert(trianglePosition.end(), newPos.begin(), newPos.end());

        return trianglePosition;*/
        return {};
    }
};