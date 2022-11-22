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

Canvas canvas;
Mode drawingMode = Draw;
double lastPixel_xpos, lastPixel_ypos;
bool mouseLeftPressed = false;
bool mouseRightPressed = false;
float pointSize = 0;
float curr_col[3] = { 1.0, 1.0, 1.0 };

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    switch (key)
    {
    case GLFW_KEY_1:
        drawingMode = Draw;
        std::cout << "Tool : Draw" << std::endl;
        break;
    case GLFW_KEY_2:
        drawingMode = Select;
        std::cout << "Tool : Select" << std::endl;
        break;
    case GLFW_KEY_3:
        drawingMode = DrawShape;
        std::cout << "Tool : Shape" << std::endl;
        break;
    case GLFW_KEY_4:
        drawingMode = Fill;
        std::cout << "Tool : Fill" << std::endl;
        break;
    default:
        break;
    }
}



static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    double adj_ypos = adjustYCoord(ypos);

    if (mouseLeftPressed && !mouseRightPressed) {
        canvas.drawLineBetween(lastPixel_xpos, lastPixel_ypos, xpos, adj_ypos,
            curr_col[0], curr_col[1], curr_col[2], pointSize);
        lastPixel_xpos = xpos;
        lastPixel_ypos = adj_ypos;
    }
    else if (mouseRightPressed && !mouseLeftPressed) {
        canvas.drawLineBetween(lastPixel_xpos, lastPixel_ypos, xpos, adj_ypos,
            DEF_COLOR_R, DEF_COLOR_G, DEF_COLOR_B, pointSize);
        lastPixel_xpos = xpos;
        lastPixel_ypos = adj_ypos;
    }
}

static void mouseButton_callback(GLFWwindow* window, int button, int action, int mods) {
    double curr_xpos, curr_ypos;
    glfwGetCursorPos(window, &curr_xpos, &curr_ypos);
    curr_ypos = adjustYCoord(curr_ypos);

    if (ImGui::IsAnyWindowHovered() || ImGui::IsAnyItemHovered()) return;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        switch (drawingMode)
        {
        case Draw:
            mouseLeftPressed = true;
            glfwGetCursorPos(window, &lastPixel_xpos, &lastPixel_ypos);
            lastPixel_ypos = adjustYCoord(lastPixel_ypos);
            break;
        case Select:
            break;
        case DrawShape:
            break;
        case Fill:
            canvas.fill(curr_xpos, curr_ypos, curr_col[0], curr_col[1], curr_col[2]);
            break;
        default:
            break;
        }
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        mouseRightPressed = true;
        glfwGetCursorPos(window, &lastPixel_xpos, &lastPixel_ypos);
        lastPixel_ypos = adjustYCoord(lastPixel_ypos);
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {

        switch (drawingMode)
        {
        case Draw:
            mouseLeftPressed = false;
            canvas.drawLineBetween(lastPixel_xpos, lastPixel_ypos, curr_xpos, curr_ypos,
                curr_col[0], curr_col[1], curr_col[2], pointSize);
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
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        mouseRightPressed = false;
        canvas.drawLineBetween(lastPixel_xpos, lastPixel_ypos, curr_xpos, curr_ypos,
            DEF_COLOR_R, DEF_COLOR_G, DEF_COLOR_B, pointSize);
    }
}


int main()
{
    Window window = Window(WINDOW_WIDTH, WINDOW_HEIGHT, "Paint", key_callback, mouseButton_callback, mouse_callback);

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

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Pixel), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Pixel), (void*)offsetof(Pixel, color));
    glBufferData(GL_ARRAY_BUFFER, canvas.pixels.size() * sizeof(Pixel), canvas.pixels.data(), GL_DYNAMIC_DRAW);

    
    while (window.checkEvent())
    {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //ImGui::ShowDemoWindow();
        ImGui::SetNextWindowSize(ImVec2(300, 80));
        ImGui::Begin("Paint");
        ImGui::SliderFloat("Pen size", &pointSize, POINT_SIZE_MIN, POINT_SIZE_MAX);
        ImGui::ColorEdit3("Pen Color", curr_col);
        ImGui::End();
        
        glClear(GL_COLOR_BUFFER_BIT);
        glBufferData(GL_ARRAY_BUFFER, canvas.pixels.size() * sizeof(Pixel), canvas.pixels.data(), GL_DYNAMIC_DRAW);
        glPointSize(10);
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
