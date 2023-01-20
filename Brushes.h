#pragma once
#include "Point.h"
#include "Algorithm.h"



class Brush {
public:
    std::shared_ptr < Point> point;
    int step;
    std::string name;

	virtual std::vector<Position<int>> getLine(Position<int> pos1, Position<int> pos2, int size) {
		int dx = abs(pos2.xpos - pos1.xpos);
		int dy = abs(pos2.ypos - pos1.ypos);
		if (dx > dy)
			return bresenhamLine(pos1, pos2, dx, dy, 0, *this->point, this->step, size);
		else
			return bresenhamLine(pos1.reverse(), pos2.reverse(), dy, dx, 1, *this->point, this->step, size);
	}

    virtual Color changePixelColor(Color oldColor, Color newColor) = 0;
};


class NormalBrush : public Brush {
public:
    NormalBrush() {
        this->point = std::make_shared < CircleP>();
        this->step = 1;
        this->name = "Normal Brush";
    }

    Color changePixelColor(Color oldColor, Color newColor) { return newColor; }
};


class CalligraphyBrush : public Brush {
public:

    CalligraphyBrush() {
        this->point = std::make_shared < SquareP>();
        this->step = 1;
        this->name = "Calligraphy Brush";
    }

    std::vector<Position<int>> getLine(Position<int> pos1, Position<int> pos2, int size) {
        int dx = abs(pos2.xpos - pos1.xpos);
        int dy = abs(pos2.ypos - pos1.ypos);
        if (dx > dy)
            return bresenhamLine(pos1, pos2, dx, dy, 0, *this->point, size, size);
        else
            return bresenhamLine(pos1.reverse(), pos2.reverse(), dy, dx, 1, *this->point, size, size);
    }

    Color changePixelColor(Color oldColor, Color newColor) { return newColor; }
};

class AirBrush : public Brush {
public:
    AirBrush() {
        this->point = std::make_shared < CircleRandomP>();
        this->step = 1;
        this->name = "Air Brush";
    }

    Color changePixelColor(Color oldColor, Color newColor) { return newColor; }
};

class Marker : public Brush {
public:
    Marker() {
        this->point = std::make_shared < SquareP>();
        this->step = 1;
        this->name = "Marker";
    }

    Color changePixelColor(Color oldColor, Color newColor) { return newColor; }
};


