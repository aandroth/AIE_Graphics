#version 430

layout(location = 3) uniform mat4 lproj;
layout(location = 4) uniform mat4 lview;
layout(location = 5) uniform sampler2D shadowMap;

in vec2 vUV;
in vec4 vPos;

uniform float shadowBias;

// Simple matrix that converts from clip-space (-1, 1) to UV space (0, 1)
uniform mat4 clipToUV = mat4(0.5f, 0.0f, 0.0f, 0.0f
							0.0f, 0.5f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.5f, 0.0f
							0.5f, 0.5f, 0.5f, 1.0f);

// The UV space (0, 1)
// This is a 2D position, with the z being the actual depth of the pixel
vec4 sUV = clipToUV * lproj * lview * vPos;

out vec4 outColor;

void main()
{

	float visibility = 1;
	if(texture(shadowmap, sUV.xy).r) < sUV.z - shadowBias)
		visibility = 0;
	
	outColor = vec4(1, 1, 0, 1) * visibility;
}