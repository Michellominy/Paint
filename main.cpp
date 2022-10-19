#include "Window.h";
#include "Shader.h"
#include "Canvas.h"



//TODO: appliquer le parametre size


Canvas canvas;
double lastPixel_xpos, lastPixel_ypos;
bool mouseLeftPressed = false;
float pointSize = 10.0;
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
        std::cout << "Color : " << "(" << curr_colR << ", " << curr_colG << ", " << curr_colB << ")" <<std::endl;
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
    default:
        break;
    }
}



static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

    // TODO:
    // dessiner tous les pixels entre ces pixels
    // Update le buffer a l'aide de glBufferSubData
    if (mouseLeftPressed) {
        // TODO: OUT OF RANGE
        canvas.drawBetween(lastPixel_xpos, abs(lastPixel_ypos - WINDOW_HEIGHT), xpos, abs(ypos - WINDOW_HEIGHT),
            curr_colR, curr_colG, curr_colB);
        lastPixel_xpos = xpos;
        lastPixel_ypos = ypos;
    }
}

static void mouseButton_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        mouseLeftPressed = true;
        glfwGetCursorPos(window, &lastPixel_xpos, &lastPixel_ypos);
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        double curr_xpos, curr_ypos;
        mouseLeftPressed = false;
        glfwGetCursorPos(window, &curr_xpos, &curr_ypos);
        canvas.drawBetween(lastPixel_xpos, abs(lastPixel_ypos - WINDOW_HEIGHT), curr_xpos, abs(curr_ypos - WINDOW_HEIGHT),
            curr_colR, curr_colG, curr_colB);
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

    //glPointSize(10.0);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)offsetof(Point, color));
    glBufferData(GL_ARRAY_BUFFER, canvas.points.size() * sizeof(Point), canvas.points.data(), GL_DYNAMIC_DRAW);
    while (window.checkEvent())
    {

        glClear(GL_COLOR_BUFFER_BIT);
        //glEnable(GL_PROGRAM_POINT_SIZE); // Pense pas que necessaire
       // glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, canvas.points.size() * sizeof(Point), canvas.points.data(), GL_DYNAMIC_DRAW);
        glPointSize(10);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, canvas.points.size());
        glBindVertexArray(0);

        window.renderLoop();
    }

    window.close();
    return 0;
}
