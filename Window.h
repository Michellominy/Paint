#pragma once
#include <GL/glew.h>
#include <process.h>
#include <iostream>
#include <GLFW/glfw3.h>

class Window {
public:
    GLFWwindow* window_;

    Window(int width, int height, const char* title, GLFWmousebuttonfun mouseButtonFunc, GLFWcursorposfun mouseFunc) {
        if (!glfwInit())
            exit(1);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window_ = glfwCreateWindow(width, height, title, NULL, NULL);
        if (!window_) // En cas d'erreur
        {
            glfwTerminate();
            exit(1);
        }
        glfwMakeContextCurrent(window_);
        glewExperimental = GL_TRUE;
        glewInit();
        glfwSetMouseButtonCallback(window_, mouseButtonFunc);
        glfwSetCursorPosCallback(window_, mouseFunc);
    }

    void close() {
        glfwDestroyWindow(window_);
        glfwTerminate();
    }

    int checkEvent() {
        return !glfwWindowShouldClose(window_);
    }

    void renderLoop() {
        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
};