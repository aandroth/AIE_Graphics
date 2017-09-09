#version 430

layout(location = 3) uniform mat4 lproj;
layout(location = 4) uniform mat4 lview;
layout(location = 5) uniform vec3 lightDir;
layout(location = 6) uniform sampler2D shadowmap;
layout(location = 7) uniform sampler2D diffuse;

in vec2 vUV;
in vec4 vPos;
in vec4 vNormal;

uniform float shadowBias = 0.01f;

// Simple matrix that converts from clip-space (-1, 1) to UV space (0, 1)
uniform mat4 clipToUV = mat4(0.5f, 0.0f, 0.0f, 0.0f,
							0.0f, 0.5f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.5f, 0.0f,
							0.5f, 0.5f, 0.5f, 1.0f);

// The UV space (0, 1)
// This is a 2D position, with the z being the actual depth of the pixel
vec4 sUV = clipToUV * lproj * lview * vPos;

out vec4 outColor;

void main()
{
	float visibility = 5.0f;
	if(texture(shadowmap, sUV.xy).r < sUV.z - shadowBias)
		visibility = 0.0f;
	
	vec3 L = normalize(lightDir); // Light direction
	vec3 N = vNormal.xyz;
	float lamb = max(dot(N, -L), 0.0f); // Keep the result between 0 and 1

	outColor = vec4(1.0f, 1.0f, 1.0f, 1.0f) * texture(diffuse, vUV) * visibility * lamb;
}