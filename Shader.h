#pragma once

#include <GL/glew.h>
#include <string>
#include <fstream>
#include "common.h"



class Shader {
private:
    unsigned int vertexSaderID;
    unsigned int fragmentShaderID;

public:

    Shader() {
        const char* vertexShaderSource = getShaderSource("vertex");
        this->vertexSaderID = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(this->vertexSaderID, 1, &vertexShaderSource, NULL);

        const char* fragmentShaderSource = getShaderSource("fragment");
        this->fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(this->fragmentShaderID, 1, &fragmentShaderSource, NULL);
    }

    void compileShaders() {
        glCompileShader(this->vertexSaderID);
        glCompileShader(this->fragmentShaderID);

        // TODO: Afficher les erreurs de compilation
    }

    void attachShaders(unsigned int prog) {
        glAttachShader(prog, this->vertexSaderID);
        glAttachShader(prog, this->fragmentShaderID);
    }

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

    ~Shader() {
        glDeleteShader(this->vertexSaderID);
        glDeleteShader(this->fragmentShaderID);
    }
};

