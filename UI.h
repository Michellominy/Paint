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

	void drawMenu(Mode &drawingMode, Shape *selectedShape, int &pointSize, Color &curr_col, Canvas &canvas) {
		ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT / 10));
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::Begin("Menu", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
		{
			if (ImGui::Button("Undo"))
				canvas.undo();

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
				if (ImGui::Selectable("Rectangle", dynamic_cast<Rectangle*>(selectedShape) && drawingMode == DrawShape)) { drawingMode = DrawShape; selectedShape = new Rectangle; }
				/*if (ImGui::Selectable("Circle", dynamic_cast<Circle*>(selectedShape) && drawingMode == DrawShape)) { drawingMode = DrawShape; selectedShape = new Circle; }
				if (ImGui::Selectable("Triangle", dynamic_cast<Triangle*>(selectedShape) && drawingMode == DrawShape)) { drawingMode = DrawShape; selectedShape = new Triangle; }*/
				ImGui::EndPopup();
			}ImGui::SameLine();

			ImGui::SetNextItemWidth(WINDOW_WIDTH / 5);
			ImGui::SliderInt("Size", &pointSize, POINT_SIZE_MIN, POINT_SIZE_MAX); ImGui::SameLine();
			ImGui::ColorEdit4("Color", &curr_col.r, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
		}
		ImGui::End();
	}

};