#include "Application.h"

Application* Application::Application_ = nullptr;

Application* Application::GetInstance()
{
	if (Application_ == nullptr) {
		Application_ = new Application();
	}
	return Application_;
}

void Application::loop() {
	while (this->window->checkEvent())
	{
		this->ui->newFrame();

		//ImGui::ShowDemoWindow();

		this->ui->drawMenu(this->currColor, this->currSize, *this->canvas);

		this->glprog->render(this->canvas->pixels.size(), this->canvas->pixels.data());

		this->ui->render();
		this->window->renderLoop();
	}
}

void Application::mouseButton_callback(GLFWwindow* window, int button, int action, int mods) {
	Application* app = Application::GetInstance();

	glfwGetCursorPos(window, &app->mouse.currPosition.xpos, &app->mouse.currPosition.ypos);
	app->mouse.currPosition.ypos = adjustYCoord(app->mouse.currPosition.ypos);

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_RELEASE) {
			app->mouse.releasedButton.left = true;
			app->mouse.pressedButton.left = false;
		}
		else if (action == GLFW_PRESS) {
			app->mouse.releasedButton.left = false;
			app->mouse.pressedButton.left = true;
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (action == GLFW_RELEASE) {
			app->mouse.releasedButton.right = true;
			app->mouse.pressedButton.right = false;
		}
		else if (action == GLFW_PRESS) {
			app->mouse.releasedButton.right = false;
			app->mouse.pressedButton.right = true;
		}
	}


	if (app->mouse.onlyOneButtonPressed())
		app->handleMousePressed();
	else if (app->mouse.onlyOneButtonPressed())
		app->handleMouseReleased();

}

void Application::mouseMovement_callback(GLFWwindow* window, double xpos, double ypos) {
	Application* app = Application::GetInstance();
	app->mouse.currPosition = { xpos, adjustYCoord(ypos) };
	app->handleMouseMovement();
}