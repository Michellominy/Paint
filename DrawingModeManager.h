#pragma once
#include "DrawingMode.h"

class DrawingModeManager {
public:
	std::vector<std::shared_ptr<DrawingMode>> modeList;
	std::shared_ptr<DrawingMode> currentMode;

	DrawingModeManager() {
		this->modeList = {
			std::make_shared <Draw>(),
			std::make_shared <Select>(),
			std::make_shared <DrawShape>(),
			std::make_shared <Fill>()
		};

		this->currentMode = this->modeList.front();
	}

	bool isCurrent(std::string modeName) {
		return this->currentMode->name == modeName;
	}

	void changeCurrent(std::string modeName) {
		for (auto mode : this->modeList)
			if (mode->name == modeName) {
				this->currentMode = mode;
				break;
			}
	}
};