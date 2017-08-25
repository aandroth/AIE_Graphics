#version 450
		in vec4 vPos; 
		in vec4 vCol; 
		in vec2 time; 

		layout(location = 0) uniform mat4 vModel; 

		void main()\n{
		vec2 outUV = vUV;
		outColor = texture(map, outUV.xy);
		};

#version 450

layout(location = 0) in vec4 pos;
layout(location = 1) in vec4 col;
layout(location = 2) in float time;

layout(location = 0) uniform mat4 model;

out vec4 vCol;

void main()
{
	gl_Position = model * pos * glm::rotate(model, sin(time)*360.0f, glm::vec3(1, 1, 0));
	
	vCol = col;
}