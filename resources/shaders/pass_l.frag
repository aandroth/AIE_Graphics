#version 430

// We can create all new visual styles using this, 
// such as sharpening the light or creating a blur.
// This may apply to all of the passes, and not just this.

in vec2 vUV;

layout(location = 0) uniform mat4 clipToUVLightProjLightViewInverseCamView_Matrix;

layout(location = 1) uniform mat4 lightColor;
layout(location = 2) uniform float intensity;

layout(location = 3) uniform sampler2D normalMap;
layout(location = 4) uniform sampler2D positionMap;

clipToUVLightProj

layout(location = 0) out vec4 outDiffuse;

uniform float shadowBias;

void main()
{
	shadowBias = 0.01f;

	vec4 vPos = texture(positionMap, vUV);
	vec4 sUV = clipToUVLightProjLightViewInverseCamView_Matrix * vPos;

	float visibility = 1;
	if(texture(shadowMap, sUV).r < sUV.z - shadowBias)
		visibility = 0;

	vec3 L = (view * -lightView[2]).xyz;
	vec3 N = texture(normalMap, vUV).xyz;
	float lamb = max(0, dot(-L, N));

	outDiffuse = lightColor * intensity * lamb * visibility;
}