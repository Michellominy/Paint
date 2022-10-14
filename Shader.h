#pragma once

#include <GL/glew.h>
#include <string>
#include <fstream>


#define SHADER_EXTENSION ".glsl"

const char* getShaderSource(std::string shaderName);

class Shader {
private:
    unsigned int vertexSaderID;
    unsigned int fragmentShaderID;

public:

    Shader() {
        const char* vertexShaderSource = getShaderSource("vertex");
        vertexSaderID = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexSaderID, 1, &vertexShaderSource, NULL);

        const char* fragmentShaderSource = getShaderSource("fragment");
        fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShaderID, 1, &fragmentShaderSource, NULL);
    }

    void compileShaders() {
        glCompileShader(vertexSaderID);
        glCompileShader(fragmentShaderID);

        // TODO: Afficher les erreurs de compilation
    }

    void attachShaders(unsigned int prog) {
        glAttachShader(prog, vertexSaderID);
        glAttachShader(prog, fragmentShaderID);
    }

    ~Shader() {
        glDeleteShader(vertexSaderID);
        glDeleteShader(fragmentShaderID);
    }
};



const char* getShaderSource(std::string shaderName) {
    std::string shaderString;

    std::ifstream file(shaderName + SHADER_EXTENSION);
    for (std::string currLine; std::getline(file, currLine);) shaderString.append(currLine + '\n');
    file.close();

    int size = shaderString.size() + 1;
    char* shaderSource = new char[size];
    strncpy_s(shaderSource, size, shaderString.c_str(), size);

    return shaderSource;
}