#pragma once
#include "Canvas.h"


class DrawingMode {
public:
	std::string name;
	virtual void  mousePressed(Canvas& canvas, Brush& currentBrush, Shape& currentShape, Color col, int size, Mouse& mouse) = 0;
	virtual void  mouseMoved(Canvas& canvas, Brush& currentBrush, Shape& currentShape, Color col, int size, Mouse& mouse) = 0;
	virtual void  mouseReleased(Canvas& canvas, Brush& currentBrush, Shape& currentShape, Color col, int size, Mouse& mouse) = 0;
};


class Draw : public DrawingMode {
public:
	Draw() {
		this->name = "Draw";
	}

	void mousePressed(Canvas& canvas, Brush& currentBrush, Shape& currentShape, Color col, int size, Mouse& mouse) {
		canvas.save();
		std::vector<Position<int>> pointPos = currentBrush.point->getPosition(mouse.currPosition, size);
		canvas.draw(pointPos, mouse.pressedButton.left ? col : Color{ DEF_CANVAS_COLOR_R, DEF_CANVAS_COLOR_G, DEF_CANVAS_COLOR_B, DEF_CANVAS_COLOR_A });
		mouse.lastPosition = mouse.currPosition;
	}

	void  mouseMoved(Canvas& canvas, Brush& currentBrush, Shape& currentShape, Color col, int size, Mouse& mouse) {
		std::vector<Position<int>> brushPos = currentBrush.getLine(mouse.currPosition, mouse.lastPosition, size);
		canvas.draw(brushPos, mouse.pressedButton.left ? col : Color{ DEF_CANVAS_COLOR_R, DEF_CANVAS_COLOR_G, DEF_CANVAS_COLOR_B, DEF_CANVAS_COLOR_A });
		mouse.lastPosition = mouse.currPosition;
	}


	void  mouseReleased(Canvas& canvas, Brush& currentBrush, Shape& currentShape, Color col, int size, Mouse& mouse) {

	}
};


class Select : public DrawingMode {
public:
	Select() {
		this->name = "Select";
	}

	void mousePressed(Canvas& canvas, Brush& currentBrush, Shape& currentShape, Color col, int size, Mouse& mouse) {
		
	}

	void  mouseMoved(Canvas& canvas, Brush& currentBrush, Shape& currentShape, Color col, int size, Mouse& mouse) {

	}

	void  mouseReleased(Canvas& canvas, Brush& currentBrush, Shape& currentShape, Color col, int size, Mouse& mouse) {

	}
};
	
	
class DrawShape : public DrawingMode {
public:
	DrawShape() {
		this->name = "DrawShape";
	}

	void mousePressed(Canvas& canvas, Brush& currentBrush, Shape& currentShape, Color col, int size, Mouse& mouse) {
		canvas.save();
		mouse.lastPosition = mouse.currPosition;
	}

	void  mouseMoved(Canvas& canvas, Brush& currentBrush, Shape& currentShape, Color col, int size, Mouse& mouse) {
		canvas.undo(false);
		std::vector<Position<int>> shapePos = currentShape.getPosition(mouse.lastPosition, mouse.currPosition, currentBrush, size);
		canvas.draw(shapePos, col);
	}

	void  mouseReleased(Canvas& canvas, Brush& currentBrush, Shape& currentShape, Color col, int size, Mouse& mouse) {
		canvas.undo(false);
		std::vector<Position<int>> shapePos = currentShape.getPosition(mouse.lastPosition, mouse.currPosition, currentBrush, size);
		canvas.draw(shapePos, col);
	}
};
	
	
class Fill : public DrawingMode {
public:
	Fill() {
		this->name = "Fill";
	}

	void mousePressed(Canvas& canvas, Brush& currentBrush, Shape& currentShape, Color col, int size, Mouse& mouse) {
		canvas.save();
		canvas.fill(mouse.currPosition, col);
	}

	void  mouseMoved(Canvas& canvas, Brush& currentBrush, Shape& currentShape, Color col, int size, Mouse& mouse) {}

	void  mouseReleased(Canvas& canvas, Brush& currentBrush, Shape& currentShape, Color col, int size, Mouse& mouse) {}
};