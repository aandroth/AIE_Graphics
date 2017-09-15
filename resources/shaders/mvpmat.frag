#version 430

out vec4 outColor;
in vec2 vUV;
in vec4 vPos;
in vec4 vNormal;
in mat4 vTBN;
in float quadrant;

layout(location = 3) uniform sampler2D diffuse;
layout(location = 4) uniform sampler2D normalMap;
layout(location = 5) uniform float cutoff;

void main()
{
	vec3 L = normalize(vec3(1, -1, 1)); // Light direction
	vec3 N = (vTBN * (2*texture(normalMap, vUV)-1)).xyz;

	// Diffuse lighting
	// Consider the interaction with a polygon normal and the opposite vector of the light
	// dot(<0, 1>, <0,  1>) =  1; parallel, fully lit polygon
	// dot(<0, 1>, <0, -1>) = -1; opposite, casts a shadow lit polygon
	// dot(<1, 0>, <0,  1>) =  0, perpendicular, no lighting or shadow
	// The smaller the angle, the less bright the polygon is
	float lamb = max(dot(N, -L), 0);

	//outColor = vec4(0, 1, 1, 1);
	if(vPos[0].x < cutoff)
		outColor = texture(diffuse, vUV);
	else
		outColor = lamb * texture(diffuse, vUV);
	//outColor = normalize(-vNormal); // turns our model's colors into the normals, just to see them
	//outColor = vec4(0, 1, 1, 1);
}