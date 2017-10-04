#version 430

layout(location = 0) in vec3  l_data;
layout(location = 0) in vec4  l_color;
layout(location = 0) in float l_intensity;
layout(location = 0) in vec4  l_ambient;
layout(location = 0) in int   l_type;

out vec2 vUV;

layout(location = 0) uniform mat4 projViewModelMatrix;

void main()
{

	vPos = model * position;

	vNormal = model * normal;

	gl_Position = projViewModelMatrix * position;

	vUV = texCoord;
}