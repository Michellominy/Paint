#pragma once
#include "Window.h"
#include "UI.h"
#include "OpenGL.h"
#include "Mouse.h"

class Application {
private:
	static Application* Application_;
	std::shared_ptr < Window > window;
	std::shared_ptr < GL> glprog;
	std::shared_ptr < UI> ui;
	std::shared_ptr < Canvas> canvas;
	std::shared_ptr < DrawingModeManager> modes;
	std::shared_ptr < BrushManager> brushes;
	std::shared_ptr < ShapeManager> shapes;
	Color currColor;
	int currSize;
	Mouse mouse;

	Application() {
		srand(time(NULL));
		this->window = std::make_shared<Window>(WINDOW_WIDTH, WINDOW_HEIGHT, "Paint", Application::mouseButton_callback, Application::mouseMovement_callback);
		this->canvas = std::make_shared < Canvas>();
		this->modes = std::make_shared < DrawingModeManager>();
		this->brushes = std::make_shared < BrushManager>();
		this->shapes = std::make_shared < ShapeManager>();
		this->ui = std::make_shared < UI>(this->window->window_, this->modes, this->brushes, this->shapes);
		this->glprog = std::make_shared < GL>(this->canvas->pixels.size(), this->canvas->pixels.data());
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

	void handleMousePressed() {
		if (Canvas::isInCanvas(this->mouse.currPosition) && this->mouse.onlyOneButtonPressed() && ! UI::IsOnMenu())
			modes->currentMode->mousePressed(*this->canvas, *this->brushes->currentBrush, *this->shapes->currentShape, this->currColor, this->currSize, this->mouse);
	}

	void handleMouseMovement() {
		if (Canvas::isInCanvas(this->mouse.currPosition) && this->mouse.onlyOneButtonPressed() && !UI::IsOnMenu())
			modes->currentMode->mouseMoved(*this->canvas, *this->brushes->currentBrush, *this->shapes->currentShape, this->currColor, this->currSize, this->mouse);
	}

	void handleMouseReleased() {
		if (Canvas::isInCanvas(this->mouse.currPosition) && !UI::IsOnMenu())
			modes->currentMode->mouseReleased(*this->canvas, *this->brushes->currentBrush, *this->shapes->currentShape, this->currColor, this->currSize, this->mouse);
	}

	~Application() {
		this->ui->close();
		this->window->close();
		delete Application_;
	}
};
