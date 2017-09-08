
#version 430

out vec4 outColor;
in vec2 vUV;

layout(location = 0)uniform sampler2D gDiffuse;
layout(location = 1)uniform sampler2D lDiffuse;

void main()
{
	// We're multiplying the color of the geometry by the color of the light.
	outColor = texture(gDiffuse, vUV.xy) * texture(lDiffuse, vUV.xy);
}