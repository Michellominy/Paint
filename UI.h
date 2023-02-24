#pragma once
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "BrushManager.h"
#include "ShapeManager.h"
#include "DrawingModeManager.h"

class UI
{
private:
	std::shared_ptr < DrawingModeManager> modes;
	std::shared_ptr < BrushManager> brushes;
	std::shared_ptr < ShapeManager> shapes;


public:

	UI(GLFWwindow* glfwWindow, std::shared_ptr < DrawingModeManager> modes, std::shared_ptr < BrushManager> brushes, std::shared_ptr < ShapeManager> shapes) {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
		ImGui_ImplOpenGL3_Init(GLSL_VERSION);

		this->brushes = brushes;
		this->shapes = shapes;
		this->modes = modes;
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

	void drawMenu(Color& curr_col, int& curr_size, Canvas& canvas) {
		ImGui::SetNextWindowSize(ImVec2(UI_WIDTH, UI_HEIGHT));
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::Begin("Menu", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
		{
			if (ImGui::Button("Undo"))
				canvas.undo();

			this->toolsButton();
			this->brushesButton();
			this->shapesButton();

			ImGui::SetNextItemWidth(UI_WIDTH / 3);
			ImGui::SliderInt("Size", &curr_size, POINT_SIZE_MIN, POINT_SIZE_MAX); ImGui::SameLine();
			ImGui::ColorEdit4("Color", &curr_col.r, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
		}
		ImGui::End();
	}

	void toolsButton() {
		if (ImGui::Button("Tools"))
			ImGui::OpenPopup("ToolsPopUp");

		if (ImGui::BeginPopup("ToolsPopUp")) {
			for (auto mode : this->modes->modeList)
				if (ImGui::Selectable(mode->name.data(), this->modes->isCurrent(mode->name)))
					this->modes->changeCurrent(mode->name);
			ImGui::EndPopup();
		}ImGui::SameLine();
	}

	void brushesButton() {
		if (ImGui::Button("Brushes"))
			ImGui::OpenPopup("BrushesPopUp");

		if (ImGui::BeginPopup("BrushesPopUp")) {
			for (auto brush : this->brushes->brushList)
				if (ImGui::Selectable(brush->name.data(), this->brushes->isCurrent(brush->name)))
					this->brushes->changeCurrent(brush->name);
			ImGui::EndPopup();
		}ImGui::SameLine();
	}

	void shapesButton() {
		if (ImGui::Button("Shape"))
			ImGui::OpenPopup("ShapePopUp");

		if (ImGui::BeginPopup("ShapePopUp")) {
			for (auto shape : this->shapes->shapeList)
				if (ImGui::Selectable(shape->name.data(), this->shapes->isCurrent(shape->name)))
					this->shapes->changeCurrent(shape->name);
			ImGui::EndPopup();
		}ImGui::SameLine();
	}

	static bool IsOnMenu() { return ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow | ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) || ImGui::IsAnyItemHovered(); }
};