// Graphics\resouces\shaders\shadow.vert
#version 430

layout(location = 0) in vec4 position;

layout(location = 0) uniform mat4 l_proj;
layout(location = 1) uniform mat4 l_view;
layout(location = 2) uniform mat4 model;

void main()
{
	gl_Position = l_proj * l_view * model * position;
}