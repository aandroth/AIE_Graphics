#version 450

layout(location = 0) unifoirm vec4 position;
layout(location = 2) unifoirm vec4 texcoord;

out vec2 vUV;

void main()
{
	gl_Postion = position;
	vUV = texcoord;
}