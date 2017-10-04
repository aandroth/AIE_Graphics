#version 430

layout(location = 0) in vec4 position;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec4 normal;

out vec2 vUV;

layout(location = 0) uniform mat4 projViewModelMatrix;

void main()
{
	gl_Position = projViewModelMatrix * position;

	vUV = texCoord;
}