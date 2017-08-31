#version 450

in vec2 vUV;

layout(location = 2)uniform mat4 model;
layout(location = 3)uniform sampler2D normalMap;
layout(location = 4)uniform sampler2D diffuseMap;
layout(location = 5)uniform vec3 light;

out vec4 outColor;

float lamb(vec normal, vec3 light)
{
	return max(0, dot(normal, -light));
}

float phong(vec normal, vec3 light, vec3 camView, float power)
{
	vec3 reflection = reflect(-light, normal);
	float base = max(0, dot(camView, reflection));

	return pow(base, power);
}

void main()
{
	vec4 texureNormal = 1 - 2*texture(1 - 2*normalMap, uUV);
	textureNormal.a = 0;
	
	vec3 normal = (vTBN * textureNormal).xyz;
	vec3 camView = model[2].xyz;
	
	float lamb = lamb(normal, light);
	float spec = phong(normal, light, camView, 1.0);

	// Test the colors
	outColor = texture(diffuseMap, vUV) * lamb + vec4(1, 1, 0, 1) * spec;
}