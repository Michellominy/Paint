#pragma once
#include "Shader.h"
#include <memory>

class GL {
public:
	std::unique_ptr <Shader> shader;
	unsigned int shaderProgram;
	unsigned int VAO, VBO;

	GL(unsigned int pointCount, Pixel* data) {
		this->shader = std::make_unique<Shader>();
		this->shader->compileShaders();

		this->shaderProgram = glCreateProgram();
		this->shader->attachShaders(this->shaderProgram);
		glLinkProgram(this->shaderProgram);

		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);

		glBindVertexArray(this->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, sizeof(Pixel), (void*)0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Pixel), (void*)offsetof(Pixel, color));
		glBufferData(GL_ARRAY_BUFFER, pointCount * sizeof(Pixel), data, GL_DYNAMIC_DRAW);
	}

	void render(unsigned int pointCount, Pixel* data) {
		glClear(GL_COLOR_BUFFER_BIT);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Pixel) * pointCount, data, GL_DYNAMIC_DRAW);

		glUseProgram(this->shaderProgram);
		glBindVertexArray(this->VAO);
		glDrawArrays(GL_POINTS, 0, pointCount);
		glBindVertexArray(0);
	}
};

