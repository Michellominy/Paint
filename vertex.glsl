#version 330 core

layout (location = 0) in vec2 pos;

out vec3 vertexColor;

void main()
{
	gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);
	gl_PointSize = 10.0;
}