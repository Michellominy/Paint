#include "Window.h";
#include "Shader.h"
#include "Canvas.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

// TODO: implementer Select
// TODO: ajouter un vrai canvas a l'interieur de la fenetre
// TODO: implementer l'ajout de forme
// TODO: Clean le code (struct, param fonction,....)
// TODO: automatically give version of glsl to ImGui
// TODO: Add Shape selector
// TODO: Optimize Fill algorithm
// TODO: regler bug si on slide de l'interieur d'une fenetre et unclick a l'exterieur
// TODO: Antialiasing

Canvas canvas;
Mode drawingMode = Draw;
Position<double> mouseLastPixel;
bool mouseLeftPressed = false;
bool mouseRightPressed = false;
float pointSize = 0;
Color curr_col = { 1.0, 1.0, 1.0, 1.0 };

static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    Position<double> adjustedPos = { xpos, adjustYCoord(ypos) };

    if (mouseLeftPressed && !mouseRightPressed) { 
        canvas.drawLineBetween(mouseLastPixel, adjustedPos, curr_col, pointSize);
        mouseLastPixel = adjustedPos;
    }
    else if (mouseRightPressed && !mouseLeftPressed) {
        canvas.drawLineBetween(mouseLastPixel, adjustedPos,
            { DEF_COLOR_R, DEF_COLOR_G, DEF_COLOR_B, DEF_COLOR_A }, pointSize);
        mouseLastPixel = adjustedPos;
    }
}

static void mouseButton_callback(GLFWwindow* window, int button, int action, int mods) {
    Position<double> mouseCurrPos;
    glfwGetCursorPos(window, &mouseCurrPos.xpos, &mouseCurrPos.ypos);
    mouseCurrPos.ypos = adjustYCoord(mouseCurrPos.ypos);

    if (ImGui::IsAnyWindowHovered() || ImGui::IsAnyItemHovered()) {
        mouseLastPixel.xpos = -1.0;
        mouseLastPixel.ypos = -1.0;
        return; 
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        switch (drawingMode)
        {
        case Draw:
            mouseLeftPressed = true;
            glfwGetCursorPos(window, &mouseLastPixel.xpos, &mouseLastPixel.ypos);
            mouseLastPixel.ypos = adjustYCoord(mouseLastPixel.ypos);
            break;
        case Select:
            break;
        case DrawShape:
            break;
        case Fill:
            canvas.fill(mouseCurrPos, curr_col);
            break;
        default:
            break;
        }
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        mouseRightPressed = true;
        glfwGetCursorPos(window, &mouseLastPixel.xpos, &mouseLastPixel.ypos);
        mouseLastPixel.ypos = adjustYCoord(mouseLastPixel.ypos);
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && mouseLastPixel.xpos != -1.0 && mouseLastPixel.ypos != -1.0) {

        switch (drawingMode)
        {
        case Draw:
            mouseLeftPressed = false;
            canvas.drawLineBetween(mouseLastPixel, mouseCurrPos, curr_col, pointSize);
            break;
        case Select:
            break;
        case DrawShape:
            break;
        case Fill:
            break;
        default:
            break;
        }
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE && mouseLastPixel.xpos != -1.0 && mouseLastPixel.ypos != -1.0) {
        mouseRightPressed = false;
        canvas.drawLineBetween(mouseLastPixel, mouseCurrPos,
            { DEF_COLOR_R, DEF_COLOR_G, DEF_COLOR_B, DEF_COLOR_A }, pointSize);
    }
}


int main()
{
    Window window = Window(WINDOW_WIDTH, WINDOW_HEIGHT, "Paint", mouseButton_callback, mouse_callback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui::StyleColorsDark();



    const char* glsl_version = "#version 330";
    ImGui_ImplGlfw_InitForOpenGL(window.window_, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

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
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

       // ImGui::ShowDemoWindow();

        
        ImGui::SetNextWindowSize(ImVec2(300, 80));
        ImGui::Begin("Pen");
        ImGui::SliderFloat("Size", &pointSize, POINT_SIZE_MIN, POINT_SIZE_MAX);
        ImGui::ColorEdit4("Color", &curr_col.r, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
        ImGui::End();

        ImGui::SetNextWindowSize(ImVec2(160, 80));
        ImGui::Begin("Tools");
        if(ImGui::RadioButton("Draw", drawingMode == Draw)) drawingMode = Draw; ImGui::SameLine();
        if (ImGui::RadioButton("Select", drawingMode == Select)) drawingMode = Select;
        if (ImGui::RadioButton("Shape", drawingMode == DrawShape)) drawingMode = DrawShape; ImGui::SameLine();
        if (ImGui::RadioButton("Fill", drawingMode == Fill)) drawingMode = Fill;
        ImGui::End();

        glClear(GL_COLOR_BUFFER_BIT);
        glBufferData(GL_ARRAY_BUFFER, canvas.pixels.size() * sizeof(Pixel), canvas.pixels.data(), GL_DYNAMIC_DRAW);
        
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, canvas.pixels.size());
        glBindVertexArray(0);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        window.renderLoop();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    window.close();
    return 0;
}
