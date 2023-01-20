#pragma once
#include <vector>
#include "common.h"
#include <time.h>

class Point {
public:
	virtual std::vector<Position<int>> getPosition(Position<int> pos, int size) = 0;
};

class SquareP : public Point {
	std::vector<Position<int>> getPosition(Position<int> pos, int size) {
        std::vector<Position<int>> pointPos;

        int halfSize = size / 2;

        for (int i = -size; i < size; i++)
            for (int j = -size; j < size; j++)
                pointPos.push_back({ i + pos.xpos - halfSize, j + pos.ypos - halfSize });
        return pointPos;
	};
};

class CircleP : public Point {
    std::vector<Position<int>> getPosition(Position<int> pos, int size) {
        std::vector<Position<int>> pointPos;

        int halfSize = size / 2;

        for (int i = -size; i < size; i++)
            for (int j = -size; j < size; j++)
                if (i * i + j * j < size * size)
                    pointPos.push_back({ i + pos.xpos - halfSize, j + pos.ypos - halfSize });
        return pointPos;
    };
};

class CircleRandomP : public Point {
    std::vector<Position<int>> getPosition(Position<int> pos, int size) {
        std::vector<Position<int>> pointPos;

        int halfSize = size / 2;

        for (int i = -size; i < size; i++)
            for (int j = -size; j < size; j++)
                if (i * i + j * j < size * size && rand() % 100 > 85)
                    pointPos.push_back({ i + pos.xpos - halfSize, j + pos.ypos - halfSize });
        return pointPos;
    };
};