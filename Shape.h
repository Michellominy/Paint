#pragma once
#include <vector>
#include "Brushes.h"




class Shape {
public:
    std::string name;

    virtual std::vector<Position<int>> getPosition(Position<int> pos1, Position<int> pos2, Brush& currentBrush, int size) = 0;
};

class Line : public Shape {
public:
    Line() {
        this->name = "Line";
    }

    std::vector<Position<int>> getPosition(Position<int> pos1, Position<int> pos2, Brush& currentBrush, int size) {
        return currentBrush.getLine(pos1, pos2, size);
    }
};

class Rectangle : public Shape {
public:
    Rectangle() {
        this->name = "Rectangle";
    }

    std::vector<Position<int>> getPosition(Position<int> pos1, Position<int> pos2, Brush& currentBrush, int size) {
        std::vector<Position<int>> rectanglePosition;
        std::vector<Position<int>> newPos;
        Position<int> pos3 = { pos2.xpos, pos1.ypos };
        Position<int> pos4 = { pos1.xpos, pos2.ypos };

        newPos = currentBrush.getLine(pos1, pos3, size);
        rectanglePosition.insert(rectanglePosition.end(), newPos.begin(), newPos.end());
        newPos = currentBrush.getLine(pos1, pos4, size);
        rectanglePosition.insert(rectanglePosition.end(), newPos.begin(), newPos.end());
        newPos = currentBrush.getLine(pos2, pos3, size);
        rectanglePosition.insert(rectanglePosition.end(), newPos.begin(), newPos.end());
        newPos = currentBrush.getLine(pos2, pos4, size);
        rectanglePosition.insert(rectanglePosition.end(), newPos.begin(), newPos.end());

        return rectanglePosition;
    }
};


class Triangle : public Shape {
public:
    Triangle() {
        this->name = "Triangle";
    }

    std::vector<Position<int>> getPosition(Position<int> pos1, Position<int> pos2, Brush& currentBrush, int size) {
        std::vector<Position<int>> trianglePosition;
        std::vector<Position<int>> newPos;
        int middleXpos = pos1.xpos + abs(pos1.xpos - pos2.xpos) / 2;
        Position<int> upperVertex = { middleXpos, pos1.ypos };
        Position<int> leftVertex = { pos1.xpos, pos2.ypos };
        Position<int> rightVertex = pos2;
         
        newPos = currentBrush.getLine(upperVertex, leftVertex, size);
        trianglePosition.insert(trianglePosition.end(), newPos.begin(), newPos.end());
        newPos = currentBrush.getLine(leftVertex, rightVertex, size);
        trianglePosition.insert(trianglePosition.end(), newPos.begin(), newPos.end());
        newPos = currentBrush.getLine(rightVertex, upperVertex, size);
        trianglePosition.insert(trianglePosition.end(), newPos.begin(), newPos.end());

        return trianglePosition;
    }
};