#include "Window.h";
#include "Shader.h"
#include "Canvas.h"

#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1080
#define POINT_SIZE_DELTA 0.5
#define POINT_SIZE_MIN 1.0
#define POINT_SIZE_MAX 100.0

//TODO: apply size to point (by adding more point to Canvas)

Canvas canvas;
bool mouseLeftPressed = false;
float pointSize = 10.0;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    switch (key)
    {
    case GLFW_KEY_0:
        std::cout << "key 0 pressed" << std::endl;
    case GLFW_KEY_MINUS:
        pointSize = pointSize == 1.0 ? pointSize : pointSize - POINT_SIZE_DELTA;
        std::cout << "Pen size : " << pointSize << std::endl;
    case GLFW_KEY_EQUAL:
        pointSize = pointSize == POINT_SIZE_MAX ? pointSize : pointSize + POINT_SIZE_DELTA;
        std::cout << "Pen size : " << pointSize << std::endl;
    default:
        break;
    }
}



static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    float x, y;
    x = (float) (2*xpos/WINDOW_WIDTH - 1);  // from [0,WINDOW_WIDTH] to [-1,1]
    y = (float) (1 - 2*ypos/WINDOW_HEIGHT); // from [WINDOW_HEIGHT,0] to [-1,1]


    if (mouseLeftPressed)
        canvas.addPoint(x, y);
}

static void mouseButton_callback(GLFWwindow* window, int button, int action, int mods) {
    mouseLeftPressed = button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS;
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
    //glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)offsetof(Point, color));
    

    while (window.checkEvent())
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_PROGRAM_POINT_SIZE);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, canvas.points.size() * sizeof(float), canvas.points.data(), GL_DYNAMIC_DRAW);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, canvas.numberOfPoint);
        glBindVertexArray(0);

        window.renderLoop();
    }

    window.close();
    return 0;
}
