#version 450

layout(location = 0) in vec4 pos;
layout(location = 1) in vec4 col;
layout(location = 2) in vec2 time;
layout(location = 3) in vec4 time;

layout(location = 0) uniform mat4 model;

vec
out vec4 vCol;

void main()
{
	gl_Position = model * pos;// * glm::rotate(model, sin(time)*360.0f, glm::vec3(1, 1, 0));
	vNormal = model * nor;
	vCol = col;
}