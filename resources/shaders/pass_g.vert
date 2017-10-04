#version 430

layout(location = 0) in vec4 position;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec4 normal;

layout(location = 0) uniform mat4 projViewModelMatrix;
layout(location = 1) uniform mat4 viewModelMatrix;

out vec2 vUV;
out vec3 vPos;
out mat4 vTBN;
out vec3 vNormal;

void main()
{
	gl_Position = projViewModelMatrix * position;

	vUV = texCoord;

	vPos = projViewModelMatrix * position;

	vTBN = viewModelMatrix * mat4(tangent, bitangent, normal, vec4(0, 0, 0, 0));

	vNormal = (viewModelMatrix * normal).xyz;
}