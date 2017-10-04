#version 430

in vec2 vUV;
in vec3 vPos;
in mat4 vTBN;
in vec3 vNormal;

layout(location = 0) out vec4 outDiffuse;
layout(location = 1) out vec4 outPosition;
layout(location = 2) out vec4 outNormal;

layout(location = 2) uniform sampler2D diffuseMap;

void main()
{
	outDiffuse = texture(diffuseMap, vUV);

	outPosition = vec4(vPos, 1);

	outNormal = vec4(vNormal, 0);
}