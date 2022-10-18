#include "Window.h";
#include "Shader.h"
#include "Canvas.h"



//TODO: appliquer le parametre size


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
    
    // TODO: ajouter les pixels par 2 dans une genre de liste du canvas
    // dessiner tous les pixels entre ces pixels
    // Update le buffer a l'aide de glBufferSubData
    if (mouseLeftPressed) {
        // TODO: OUT OF RANGE
        double adjusted_ypos = abs(ypos - WINDOW_HEIGHT);
        int index = (int)(adjusted_ypos * WINDOW_WIDTH + xpos);
        // TODO: clairement une meilleur facon...
        canvas.points[index].color[0] = 1.0;
        canvas.points[index].color[1] = 0.0;
        canvas.points[index].color[2] = 0.0;
    }
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
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)offsetof(Point, color));
    glBufferData(GL_ARRAY_BUFFER, canvas.points.size() * sizeof(Point), canvas.points.data(), GL_DYNAMIC_DRAW);

    while (window.checkEvent())
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_PROGRAM_POINT_SIZE); // Pense pas que necessaire
       // glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, canvas.points.size() * sizeof(Point), canvas.points.data(), GL_DYNAMIC_DRAW);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, canvas.points.size());
        glBindVertexArray(0);

        window.renderLoop();
    }

    window.close();
    return 0;
}
