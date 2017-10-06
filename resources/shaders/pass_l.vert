#version 430

layout(location = 0) in vec4  position;
layout(location = 2) in float texcoord;

out vec2 vUV;

void main()
{
	gl_Position = position;

	vUV = texCoord;
}