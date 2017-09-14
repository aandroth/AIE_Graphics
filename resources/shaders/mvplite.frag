#version 430

out vec4 outColor;
in vec2 vUV;
in vec4 vPos;
in vec4 vNormal;

layout(location = 3) uniform sampler2D diffuse;

void main()
{
	vec3 L = normalize(vec3(1, -1, 1)); // Light direction
	vec3 N = vNormal.xyz;

	// Diffuse lighting
	// Consider the interaction with a polygon normal and the opposite vector of the light
	// dot(<0, 1>, <0,  1>) =  1; parallel, fully lit polygon
	// dot(<0, 1>, <0, -1>) = -1; opposite, casts a shadow lit polygon
	// dot(<1, 0>, <0,  1>) =  0, perpendicular, no lighting or shadow
	// The smaller the angle, the less bright the polygon is
	float lamb = max(dot(N, -L), 0);

	//outColor = texture(diffuse, vUV);
	outColor = lamb * texture(diffuse, vUV) * 5;
	//outColor = vec4(0, 1, 1, 1);
	//outColor = normalize(-vNormal); // turns our model's colors into the normals, just to see them
}