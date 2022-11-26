#pragma once
#include "common.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

class UI
{
public:
	UI(GLFWwindow* glfwWindow) {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
		ImGui_ImplOpenGL3_Init(GLSL_VERSION);

	}

	void newFrame() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void render() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void close() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void drawMenu(Mode &drawingMode, Shape &selectedShape, int &pointSize, Color &curr_col) {
		ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT / 10));
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::Begin("Menu", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
		{
			if (ImGui::Button("Tools"))
				ImGui::OpenPopup("ToolsPopUp");

			if (ImGui::BeginPopup("ToolsPopUp")) {
				if (ImGui::Selectable("Draw", drawingMode == Draw)) drawingMode = Draw;
				if (ImGui::Selectable("Select", drawingMode == Select)) drawingMode = Select;
				if (ImGui::Selectable("Fill", drawingMode == Fill)) drawingMode = Fill;
				ImGui::EndPopup();
			}ImGui::SameLine();


			if (ImGui::Button("Shape"))
				ImGui::OpenPopup("ShapePopUp");

			if (ImGui::BeginPopup("ShapePopUp")) {
				if (ImGui::Selectable("Square", selectedShape == Square && drawingMode == DrawShape)) { drawingMode = DrawShape; selectedShape = Square; }
				if (ImGui::Selectable("Rectangle", selectedShape == Rectangle && drawingMode == DrawShape)) { drawingMode = DrawShape; selectedShape = Rectangle; }
				if (ImGui::Selectable("Circle", selectedShape == Circle && drawingMode == DrawShape)) { drawingMode = DrawShape; selectedShape = Circle; }
				if (ImGui::Selectable("Triangle", selectedShape == Triangle && drawingMode == DrawShape)) { drawingMode = DrawShape; selectedShape = Triangle; }
				ImGui::EndPopup();
			}ImGui::SameLine();

			ImGui::SetNextItemWidth(WINDOW_WIDTH / 5);
			ImGui::SliderInt("Size", &pointSize, POINT_SIZE_MIN, POINT_SIZE_MAX); ImGui::SameLine();
			ImGui::ColorEdit4("Color", &curr_col.r, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
		}
		ImGui::End();
	}

};