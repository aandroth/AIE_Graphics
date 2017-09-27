#version 430

layout(location = 0) in vec4 position;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec4 normal;
layout(location = 4) in vec4 tangent;
layout(location = 5) in vec4 bitangent;

out vec2 vUV;
out vec4 vPos;
out vec4 vNormal;
out mat4 vTBN;

layout(location = 0) uniform mat4 proj;
layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 model;

void main()
{
	gl_Position = proj * view * model * position;

	vPos = model * position;
	vNormal = model * normal;
	vUV = texCoord;

	vTBN = model * mat4(tangent, bitangent, normal, vec4(0, 0, 0, 1));
}