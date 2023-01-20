#pragma once
#include "Brushes.h"
#include <memory>


class BrushManager {
public:
	std::vector<std::shared_ptr<Brush>> brushList;
	std::shared_ptr<Brush> currentBrush;

	BrushManager() {
		this->brushList = {
			std::make_shared <NormalBrush>(), 
			std::make_shared <CalligraphyBrush>(),
			std::make_shared <AirBrush>(),
			std::make_shared <Marker>()
		};

		this->currentBrush = this->brushList.front();
	}

	bool isCurrent(std::string brushName) {
		return this->currentBrush->name == brushName;
	}

	void changeCurrent(std::string brushName) {
		for (auto brush : this->brushList)
			if (brush->name == brushName) {
				this->currentBrush = brush;
				break;
			}
	}
};