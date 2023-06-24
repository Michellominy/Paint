#pragma once
#include <memory>
#include "Shape.h"
#include <string>

class ShapeManager {
public:
	std::vector<std::shared_ptr<Shape>> shapeList;
	std::shared_ptr<Shape> currentShape;

	ShapeManager() {
		this->shapeList = {
			std::make_shared <Line>(),
			std::make_shared <Rect>(),
			std::make_shared <Triangle>()
		};

		this->currentShape = this->shapeList.front();
	}

	bool isCurrent(std::string shapeName) {
		return currentShape->name == shapeName;
	}

	void changeCurrent(std::string shapeName) {
		for (auto shape : shapeList)
			if (shape->name == shapeName){
				currentShape = shape;
				break;
			}
	}
};