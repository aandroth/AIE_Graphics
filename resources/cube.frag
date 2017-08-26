#version 450


out vec4 outColor;
in vec4 vCol;
in vec2 vUV;
in vec4 vNormal;

layout(location = 2) uniform sampler2D map;

void main()
{
	outColor = texture(map, vUV);
}