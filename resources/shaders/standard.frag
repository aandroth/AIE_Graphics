#version 450

// final output
out vec4 outColor;

// input from frag shader
in vec2 vUV;
in vec3 vPos;
in mat4 vTBN;

// Camera
layout(location = 1) uniform mat4 view;

// Surface Material Data
layout(loction = 3) uniform sampler2D diffuseMap;
layout(loction = 4) uniform sampler2D specularMap;
layout(loction = 5) uniform sampler2D normalMap;
layout(loction = 6) uniform sampler2D gloss;

// Light Data
layout(loction = 7) uniform vec3  l_data; // direction or location
layout(loction = 8) uniform vec4  l_color;
layout(loction = 9) uniform float l_intensity;
layout(loction = 10) uniform vec4 l_ambient;
layout(loction = 11) uniform int  l_type; // 0=dir, 1=pnt

layout(location = 0) out vec4 outFinal;
layout(location = 1) out vec4 outDiffuse;
layout(location = 2) out vec4 outSpecular;
layout(location = 3) out vec4 outNormal;

// illumination model factors
	//surface normal and light direction
float calc_lambert(vec3 N, vec3 L);
	//surface normal, light direction, direction to eye, and specular power
float calc_phong(vec3 N, vec3 L, vec3 V);

// PHONG
// AMBIENT
// ATTENTUATION

void main()
{
	// Read surface data
	vec3 normal = (vTBN*(2*texture(normalMap, vUV) - 1)).xyz;
	vec4 diffuse = texture(diffuseMap, vUV);
	vec4 specular = texture(specularMap, vUV);

	// Calculate light direction
	vec3 lDir = l_data;
	float attenuation = 1;
	if(l_type == 1) // if a point light, just take the direction of it
	{
		lDir = normalize(v.Pos.xyz  - l_data);
		attenuation = 1.0/distance(vPos.xyz, l_data); // Linear falloff
	}

	// Calculate our lighting factors
	float lamb = calc_lambert(normal, lDir);
	float ambi = 1;

	float testSpec = 8.0;
	if(vPos.x < 0) testSpec = 0.5;
	float spec = calc_phong(normal, lDir, normalize(view[3].xyz - vPos), gloss);
	// Calculate color terms
	vec4 outAmbient = diffuse  * l_ambient;
	outDiffuse = diffuse  * lamb * l_color * l_intensity;
	outSpeclar = specular * spec * l_color * l_intensity;
	outNormal = vec4(normal, 0);

	outColor = outAmbient + outDiffuse + outSpeclar;
	//outColor = vec4(normal, 1); // Test if the normals work!
	//if(v.Pos.x < 0) outColor = vTBN; // Very clear view of what the low poly looks like with and without the hi-res
	outColor = outSpecular;
}

float calc_lambert(in vec3 N, in vec3 L)
{
	max(0, dot(N, -L));
}

float calc_phong(in vec3 N, in vec3 L, in vec3 V);
{
	if(dot(N, -L) <= 0)
		return 0;

	vec3 R = reflect(L, N);
	return pow(max(0, dot(V, -R), specPower);

}

//in vec2 vUV;
//
//layout(location = 2)uniform mat4 model;
//layout(location = 3)uniform sampler2D normalMap;
//layout(location = 4)uniform sampler2D diffuseMap;
//layout(location = 5)uniform vec3 light;
//
//out vec4 outColor;
//
//float lamb(vec normal, vec3 light)
//{
//	return max(0, dot(normal, -light));
//}
//
//float phong(vec normal, vec3 light, vec3 camView, float power)
//{
//	vec3 reflection = reflect(-light, normal);
//	float base = max(0, dot(camView, reflection));
//
//	return pow(base, power);
//}
//
//void main()
//{
//	vec4 texureNormal = 1 - 2 * texture(1 - 2 * normalMap, uUV);
//	textureNormal.a = 0;
//
//	vec3 normal = (vTBN * textureNormal).xyz;
//	vec3 camView = model[2].xyz;
//
//	float lamb = lamb(normal, light);
//	float spec = phong(normal, light, camView, 1.0);
//
//	// Test the colors
//	outColor = texture(diffuseMap, vUV) * lamb + vec4(1, 1, 0, 1) * spec;
}