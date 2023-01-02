#pragma once
#include <vector>
#include "common.h"


class ShapeManager {
public:
	static std::vector<Position<int>> getLinePosition(Position<int> pos1, Position<int> pos2) {
		int dx = abs(pos2.xpos - pos1.xpos);
		int dy = abs(pos2.ypos - pos1.ypos);
		if (dx > dy)
			return bresenhamLine(pos1, pos2, dx, dy, 0);
		else
			return bresenhamLine(pos1.reverse(), pos2.reverse(), dy, dx, 1);
	}

	static std::vector<Position<int>> getSquarePixel(Position<int> pos1, Position<int> pos2) {

	}

private:
    // https://www.geeksforgeeks.org/bresenhams-line-generation-algorithm/
    static std::vector<Position<int>> bresenhamLine(Position<int> pos1, Position<int> pos2, int dx, int dy, int decide)
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
};


