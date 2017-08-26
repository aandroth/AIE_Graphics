#version 450

layout(location = 0) in vec4 pos;
layout(location = 1) in vec4 col;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec4 nor;

layout(location = 0) uniform mat4 modelMat;
layout(location = 1) uniform float time;

out vec4 vCol;
out vec2 vUV;

void main()
{
	//gl_Position = model * pos;
	//vNormal = model * nor;
	//vCol = col;
	//vec4 rotationAngle = {time,time,time, 1};
	//vec4 texCoord0 = {texCoord.x,texCoord.y,1, 1};
	//vec4 texCoord1 = {texCoord.x,texCoord.y,.5,1};
	//vNormal = normalize(vec4(gl_Position.xyz,0));
	gl_Position =  modelMat * pos * vec4(0.1, 0.1, 0.1, 1);
	vUV = texCoord;
	//vCol = pos + texCoord1;
}