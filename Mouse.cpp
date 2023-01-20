/*#include "Mouse.h"

Position<double> Mouse::lastPosition = { -1.0, -1.0 };

void Mouse::mouseButton_callback(GLFWwindow* window, int button, int action, int mods) {
	Position<double> mouseCurrPos;
	glfwGetCursorPos(window, &mouseCurrPos.xpos, &mouseCurrPos.ypos);
	mouseCurrPos.ypos = adjustYCoord(mouseCurrPos.ypos);

	bool mouseLeftReleased = button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE;
	bool mouseRightReleased = button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE;

	bool mouseLeftPressed = button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS;
	bool mouseRightPressed = button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS;

	if (mouseLeftPressed || mouseRightPressed) {}
	//app.handleMousePressed(mouseLeftPressed, mouseRightPressed, mouseCurrPos);
	else if (mouseLeftReleased || mouseRightReleased) {}
	//app.handleMouseReleased(mouseLeftReleased, mouseRightReleased, lastPosition, mouseCurrPos);

//lastPosition = mouseCurrPos;
}

void Mouse::mouseMovement_callback(GLFWwindow* window, double xpos, double ypos) {
	Position<double> adjustedPos = { xpos, adjustYCoord(ypos) };
	//app.handleMouseMovement(mouseLeftPressed, mouseRightPressed, lastPosition, adjustedPos);
	//lastPosition = adjustedPos;
}*/
