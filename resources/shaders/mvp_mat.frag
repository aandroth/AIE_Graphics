#version 430

out vec4 outColor;
//out vec4 outNormal;
in vec2 vUV;
in vec4 vPos;
in vec4 vNormal;
in mat4 vTBN;

layout(location = 3) uniform sampler2D diffuse;
layout(location = 4) uniform sampler2D normalMap;

void main()
{
	vec3 L = normalize(vec3(1, -1, 1)); // Light direction
	vec3 N = (vTBN*(2*texture(normalMap, vUV)-1)).xyz;

	vec4 surfaceCol = vec4(1, 1, 1, 1);//texture(diffuse, vUV);

	float lambertFactor = max(dot(N, -L), 0); // Keep the result between 0 and 1

	vec4 lambertTerm = (surfaceCol * lambertFactor);

	outColor = lambertTerm;
}