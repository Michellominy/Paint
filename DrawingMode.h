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
		canvas.drawPoint(mouse.currPosition, mouse.pressedButton.left ? col : Color{DEF_CANVAS_COLOR_R, DEF_CANVAS_COLOR_G, DEF_CANVAS_COLOR_B, DEF_CANVAS_COLOR_A}, size, currentBrush);
		mouse.lastPosition = mouse.currPosition;
	}

	void  mouseMoved(Canvas& canvas, Brush& currentBrush, Shape& currentShape, Color col, int size, Mouse& mouse) {
		canvas.draw(mouse.lastPosition, mouse.currPosition, mouse.pressedButton.left ? col : Color{ DEF_CANVAS_COLOR_R, DEF_CANVAS_COLOR_G, DEF_CANVAS_COLOR_B, DEF_CANVAS_COLOR_A }, size, currentBrush);
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
		canvas.drawShape(mouse.lastPosition, mouse.currPosition, col, size, currentShape);
	}

	void  mouseReleased(Canvas& canvas, Brush& currentBrush, Shape& currentShape, Color col, int size, Mouse& mouse) {
		canvas.undo(false);
		canvas.drawShape(mouse.lastPosition, mouse.currPosition, col, size, currentShape);
	}
};
	
	
class Fill : public DrawingMode {
public:
	Fill() {
		this->name = "Fill";
	}

	void mousePressed(Canvas& canvas, Brush& currentBrush, Shape& currentShape, Color col, int size, Mouse& mouse) {
		canvas.fill(mouse.currPosition, col);
	}

	void  mouseMoved(Canvas& canvas, Brush& currentBrush, Shape& currentShape, Color col, int size, Mouse& mouse) {}

	void  mouseReleased(Canvas& canvas, Brush& currentBrush, Shape& currentShape, Color col, int size, Mouse& mouse) {}
};