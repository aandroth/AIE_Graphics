#version 430

out vec4 outColor;
in vec2 vUV;
in vec4 vPos;
in vec4 vNormal;

layout(location = 1) uniform mat4 view;
layout(location = 3) uniform sampler2D diffuse;
layout(location = 4) uniform vec3  lightDir;
layout(location = 5) uniform vec4  lightCol;
layout(location = 6) uniform float lightInt;
layout(location = 7) uniform vec4  lightAmb;
layout(location = 8) uniform float lightTyp;

layout(location = 9) uniform float objSpec;
layout(location = 10) uniform vec3 objNorm;
layout(location = 11) uniform float objGlos;

void main()
{
	vec4 surfaceCol = texture(diffuse, vUV);

	float lambertFactor = max(dot(vNormal.xyz, -lightDir), 0); // Keep the result between 0 and 1

	vec4 lambertTerm = (surfaceCol * lambertFactor) * lightCol;

	vec4 refletionVec = 2 * (lambertFactor) * vNormal.xyz + lightDir;

	cam_dir = (view * vec4(1, 1, 1, 1)) - vPos;

	float specular = pow(dot(cam_dir, reflectionVec), objSpec);

	//vec3 L = normalize(vec3(0, -1, 0)); // Light direction
	//vec3 N = vNormal.xyz;

	// Diffuse lighting
	// Consider the interaction with a polygon normal and the opposite vector of the light
	// dot(<0, 1>, <0,  1>) =  1; parallel, fully lit polygon
	// dot(<0, 1>, <0, -1>) = -1; opposite, casts a shadow lit polygon
	// dot(<1, 0>, <0,  1>) =  0, perpendicular, no lighting or shadow
	// The smaller the angle, the less bright the polygon is
	//float lamb = dot(N, -L);

	outColor = lambertTerm + lightCol * specular + lightCol * lightAmb;

	//outColor = vNormal; // turns our model's colors into the normals, just to see them
}