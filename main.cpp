#include "Window.h";
#include "Shader.h"
#include "Canvas.h"

// TODO: implementer Select
// TODO: ajouter un vrai canvas a l'interieur de la fenetre
// TODO: implementer l'ajout de forme
// TODO: implementer Fill

Canvas canvas;
Mode drawingMode = Draw;
double lastPixel_xpos, lastPixel_ypos;
bool mouseLeftPressed = false;
bool mouseRightPressed = false;
int pointSize = 0;
float curr_colR = 1.0;
float curr_colG = 1.0;
float curr_colB = 1.0;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {// TODO: Ameliorer ca...
    switch (key)
    {
    case GLFW_KEY_0:
        std::cout << "key 0 pressed" << std::endl;
        break;
    case GLFW_KEY_MINUS:
        pointSize = pointSize <= POINT_SIZE_MIN ? POINT_SIZE_MIN : pointSize - POINT_SIZE_DELTA;
        std::cout << "Pen size : " << pointSize << std::endl;
        break;
    case GLFW_KEY_EQUAL:
        pointSize = pointSize >= POINT_SIZE_MAX ? POINT_SIZE_MAX : pointSize + POINT_SIZE_DELTA;
        std::cout << "Pen size : " << pointSize << std::endl;
        break;
    case GLFW_KEY_R: // R++
        curr_colR = curr_colR >= COLOR_MAX ? COLOR_MAX : curr_colR + COLOR_DELTA;
        std::cout << "Color : " << "(" << curr_colR << ", " << curr_colG << ", " << curr_colB << ")" << std::endl;
        break;
    case GLFW_KEY_E: // R--
        curr_colR = curr_colR <= COLOR_MIN ? COLOR_MIN : curr_colR - COLOR_DELTA;
        std::cout << "Color : " << "(" << curr_colR << ", " << curr_colG << ", " << curr_colB << ")" << std::endl;
        break;
    case GLFW_KEY_G: // G++
        curr_colG = curr_colG >= COLOR_MAX ? COLOR_MAX : curr_colG + COLOR_DELTA;
        std::cout << "Color : " << "(" << curr_colR << ", " << curr_colG << ", " << curr_colB << ")" << std::endl;
        break;
    case GLFW_KEY_F: // G--
        curr_colG = curr_colG <= COLOR_MIN ? COLOR_MIN : curr_colG - COLOR_DELTA;
        std::cout << "Color : " << "(" << curr_colR << ", " << curr_colG << ", " << curr_colB << ")" << std::endl;
        break;
    case GLFW_KEY_B: // B++
        curr_colB = curr_colB >= COLOR_MAX ? COLOR_MAX : curr_colB + COLOR_DELTA;
        std::cout << "Color : " << "(" << curr_colR << ", " << curr_colG << ", " << curr_colB << ")" << std::endl;
        break;
    case GLFW_KEY_V: // B--
        curr_colB = curr_colB <= COLOR_MIN ? COLOR_MIN : curr_colB - COLOR_DELTA;
        std::cout << "Color : " << "(" << curr_colR << ", " << curr_colG << ", " << curr_colB << ")" << std::endl;
        break;
    case GLFW_KEY_1:
        drawingMode = Draw;
        std::cout << "Tool : Draw"<< std::endl;
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
            curr_colR, curr_colG, curr_colB, pointSize);
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
            canvas.fill(curr_xpos, curr_ypos, curr_colR, curr_colG, curr_colB);
            break;
        default:
            break;
        }
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        mouseRightPressed = true;
        glfwGetCursorPos(window, &lastPixel_xpos, &lastPixel_ypos);
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        
        switch (drawingMode)
        {
        case Draw:
            mouseLeftPressed = false;
            canvas.drawLineBetween(lastPixel_xpos, lastPixel_ypos, curr_xpos, curr_ypos,
                curr_colR, curr_colG, curr_colB, pointSize);
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
        glClear(GL_COLOR_BUFFER_BIT);
        glBufferData(GL_ARRAY_BUFFER, canvas.pixels.size() * sizeof(Pixel), canvas.pixels.data(), GL_DYNAMIC_DRAW);
        glPointSize(10);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, canvas.pixels.size());
        glBindVertexArray(0);

        window.renderLoop();
    }

    window.close();
    return 0;
}
