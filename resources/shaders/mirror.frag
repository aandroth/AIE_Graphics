#version 430

out vec4 outColor;
in vec2 vUV;
in vec4 vPos;
in vec4 vNormal;
in mat4 vTBN;
in float quadrant;

layout(location = 3) uniform sampler2D diffuse;

layout(location = 0) out vec4 color;

void main()
{
	color = texture(diffuse, vUV);//cam_proj * cam_view * model * position;
	//outFinal =  texture(diffuse, vUV);
}