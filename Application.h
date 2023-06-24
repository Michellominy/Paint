#pragma once
#include "Window.h"
#include "UI.h"
#include "OpenGL.h"

class Application {
private:
	static Application* Application_;
	std::shared_ptr <Window > window;
	std::shared_ptr <GL> glprog;
	std::shared_ptr <UI> ui;
	std::shared_ptr <Canvas> canvas;
	std::shared_ptr <DrawingModeManager> modes;
	std::shared_ptr <BrushManager> brushes;
	std::shared_ptr <ShapeManager> shapes;
	Color currColor;
	int currSize;
	Mouse mouse;
	std::stack<std::pair<Mouse, MouseState>> mouseStateStack;

	Application() {
		srand(time(NULL));
		this->window = std::make_shared<Window>(WINDOW_WIDTH, WINDOW_HEIGHT, "Paint", Application::mouseButton_callback, Application::mouseMovement_callback);
		this->canvas = std::make_shared <Canvas>();
		this->modes = std::make_shared <DrawingModeManager>();
		this->brushes = std::make_shared <BrushManager>();
		this->shapes = std::make_shared <ShapeManager>();
		this->ui = std::make_shared <UI>(this->window->window_, this->modes, this->brushes, this->shapes);
		this->glprog = std::make_shared <GL>(this->canvas->pixels.size(), this->canvas->pixels.data());
		this->currColor = { 0.0, 0.0, 0.0, 0.0 };
		this->currSize = DEF_SIZE;
	}

public:

	static Application* GetInstance();

	Application(Application& other) = delete;
	void operator=(const Application&) = delete;

	void loop();

	static void mouseButton_callback(GLFWwindow* window, int button, int action, int mods);

	static void mouseMovement_callback(GLFWwindow* window, double xpos, double ypos);

	void handleMouse() {
		if (this->mouseStateStack.empty()) return;

		MouseState mouseState = this->mouseStateStack.top().second;
		Mouse currentMouse = this->mouseStateStack.top().first;
		switch (mouseState)
		{
		case pressed:
			handleMousePressed(currentMouse);
			break;
		case released:
			handleMouseReleased(currentMouse);
			break;
		case moved:
			handleMouseMovement(currentMouse);
		default:
			break;
		}

		while (!this->mouseStateStack.empty()) { this->mouseStateStack.pop(); };
	}

	void handleMousePressed(Mouse currMouse) {
		if (Canvas::isInCanvas(currMouse.currPosition) && currMouse.onlyOneButtonPressed() && ! UI::IsOnMenu())
			modes->currentMode->mousePressed(*this->canvas, *this->brushes->currentBrush, *this->shapes->currentShape, this->currColor, this->currSize, currMouse);
	}

	void handleMouseMovement(Mouse currMouse) {
		if (Canvas::isInCanvas(currMouse.currPosition) && currMouse.onlyOneButtonPressed() && !UI::IsOnMenu())
			modes->currentMode->mouseMoved(*this->canvas, *this->brushes->currentBrush, *this->shapes->currentShape, this->currColor, this->currSize, currMouse);
	}

	void handleMouseReleased(Mouse currMouse) {
		if (Canvas::isInCanvas(currMouse.currPosition) && !UI::IsOnMenu())
			modes->currentMode->mouseReleased(*this->canvas, *this->brushes->currentBrush, *this->shapes->currentShape, this->currColor, this->currSize, currMouse);
	}

	~Application() {
		this->ui->close();
		this->window->close();
		delete Application_;
	}
};
