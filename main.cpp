#include "Application.h"

// TODO: implementer Select
// TODO: ajouter un vrai canvas a l'interieur de la fenetre
// TODO: Optimize Fill algorithm
// TODO: Antialiasing
// TODO: faire un iterator pour acceder au nom de Brush/Shape/DrawingModeManager Manager


void handleMousePressed(Position<double> currPos) {
	switch (drawingMode)
	{
	case Draw:
		canvas.save();
		mouseLastPixel = currPos;
		break;
	case Select:
		break;
	case DrawShape:
		canvas.save();
		mouseLastPixel = currPos;
		break;
	case Fill:
		if (mouseLeftPressed) {
			canvas.save();
			canvas.fill(currPos, curr_col);
		}
		break;
	default:
		break;
	}
}

void handleMouseReleased(bool mouseLeftReleased, bool mouseRightReleased, Position<double> currPos) {
	switch (drawingMode)
	{
	case Draw:
		mouseLeftPressed = mouseLeftReleased ? false : mouseLeftPressed;
		mouseRightPressed = mouseRightReleased ? false : mouseRightPressed;
		canvas.drawShape(mouseLastPixel, currPos, mouseLeftReleased ? curr_col : Color{ DEF_COLOR_R, DEF_COLOR_G, DEF_COLOR_B, DEF_COLOR_A }, pointSize, *new Line);
		break;
	case Select:
		break;
	case DrawShape:
		mouseLeftPressed = mouseLeftReleased ? false : mouseLeftPressed;
		mouseRightPressed = mouseRightReleased ? false : mouseRightPressed;
		canvas.undo(false);
		canvas.drawShape(mouseLastPixel, currPos, mouseLeftReleased ? curr_col : Color{ DEF_COLOR_R, DEF_COLOR_G, DEF_COLOR_B, DEF_COLOR_A }, pointSize, *selectedShape);
		break;
	case Fill:
		break;
	default:
		break;
	}
}

void handleMouseMovement(Position<double> currPos) {
	switch (drawingMode)
	{
	case Draw:
		if (mouseLeftPressed && !mouseRightPressed) {
			canvas.drawShape(mouseLastPixel, currPos, curr_col, pointSize, *new Line);
			mouseLastPixel = currPos;
		}
		else if (mouseRightPressed && !mouseLeftPressed) {
			canvas.drawShape(mouseLastPixel, currPos,
				{ DEF_COLOR_R, DEF_COLOR_G, DEF_COLOR_B, DEF_COLOR_A }, pointSize, *new Line);
			mouseLastPixel = currPos;
		}
	case Select:
		break;
	case DrawShape:
		if (mouseLeftPressed && !mouseRightPressed) {
			canvas.undo(false);
			canvas.drawShape(mouseLastPixel, currPos, curr_col, pointSize, *selectedShape);
		}
		else if (mouseRightPressed && !mouseLeftPressed) {
			canvas.undo(false);
			canvas.drawShape(mouseLastPixel, currPos,
				{ DEF_COLOR_R, DEF_COLOR_G, DEF_COLOR_B, DEF_COLOR_A }, pointSize, *selectedShape);
		}
		break;
	case Fill:
		break;
	default:
		break;
	}
}


static void mouseMovement_callback(GLFWwindow* window, double xpos, double ypos) {
	Position<double> adjustedPos = { xpos, adjustYCoord(ypos) };
	handleMouseMovement(adjustedPos);
}

static void mouseButton_callback(GLFWwindow* window, int button, int action, int mods) {

	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow | ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) || ImGui::IsAnyItemHovered()) {
		mouseLastPixel.xpos = -1.0;
		mouseLastPixel.ypos = -1.0;
		return;
	}

	Position<double> mouseCurrPos;
	glfwGetCursorPos(window, &mouseCurrPos.xpos, &mouseCurrPos.ypos);
	mouseCurrPos.ypos = adjustYCoord(mouseCurrPos.ypos);
	
	bool mouseLeftReleased = button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE;
	bool mouseRightReleased = button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE;

	mouseLeftPressed = button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS;
	mouseRightPressed = button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS;

	if (mouseLeftPressed || mouseRightPressed)
		handleMousePressed(mouseCurrPos);
	else if ((mouseLeftReleased || mouseRightReleased) && mouseLastPixel.xpos != -1.0 && mouseLastPixel.ypos != -1.0)
		handleMouseReleased(mouseLeftReleased, mouseRightReleased, mouseCurrPos);
}


int main()
{
	Window window = Window(WINDOW_WIDTH, WINDOW_HEIGHT, "Paint", mouseButton_callback, mouseMovement_callback);

	UI ui(window.window_);

	Shader shader = Shader();
	shader.compileShaders();

	unsigned int shaderProgram = glCreateProgram();
	shader.attachShaders(shaderProgram);
	glLinkProgram(shaderProgram);

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, sizeof(Pixel), (void*)0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Pixel), (void*)offsetof(Pixel, color));
	glBufferData(GL_ARRAY_BUFFER, canvas.pixels.size() * sizeof(Pixel), canvas.pixels.data(), GL_DYNAMIC_DRAW);

	while (window.checkEvent())
	{
		ui.newFrame();

		//ImGui::ShowDemoWindow();

		ui.drawMenu(drawingMode, selectedShape, pointSize, curr_col, canvas);

		glClear(GL_COLOR_BUFFER_BIT);
		glBufferData(GL_ARRAY_BUFFER, canvas.pixels.size() * sizeof(Pixel), canvas.pixels.data(), GL_DYNAMIC_DRAW);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_POINTS, 0, canvas.pixels.size());
		glBindVertexArray(0);

		ui.render();
		window.renderLoop();
	}

	ui.close();
	window.close();
	return 0;
}