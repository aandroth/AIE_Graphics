
#include "graphics\Context.h"
#include "graphics\Vertex.h"
#include "graphics\draw.h"
#include "graphics\load.h"
#include <vector>
using std::vector;

int main()
{
	Context context;
	context.init(800, 600, "GRAPHICS!!!!");

	Geometry cube = loadGeometry("../../resources/models/cube.obj");
	Texture t_magYel = loadTexture("../../resources/textures/Blue.jpg");

	const char *vsource = "#version 450\n"
		"layout(location = 0) uniform float time; \n"
		//"layout(location = 0) in vec4 position; \n"
		"layout(location = 1) in vec4 color; \n"
		"out vec4 vCol; \n"
		"out vec2 vUV; \n"
		"out vec4 vPos; \n"

		"void main()\n{\n"
		"vCol = color;\n"
		"vPos = position;\n"
		"gl_Position = position;\n"

		"vUV = position.xy;\n"
		"}\n";

	const char *fsource = "#version 450\n"
		"in vec4 vCol; \n"
		"in vec4 vPos; \n"
		"in vec2 vUV; \n"
		"out vec4 outColor; \n"
		"layout(location = 0) uniform float time; \n"
		"layout(location = 1) uniform sampler2D map; \n"

		"void main()\n{\n"
		//"outColor = vCol;\n"
		//"outColor.x *= 0.5f + abs(sin(time + gl_FragCoord.y/5.0f));\n"
		//"outColor.y *= 0.5f + abs(sin(time + (gl_FragCoord.y+0.25f)/10.0f));\n"
		//"outColor.z *= 0.5f + abs(sin(time + (gl_FragCoord.y+0.75f)/10.0f));\n"
		//"outColor.z -= abs(sin(-time + (-gl_FragCoord.y+0.75f)/10.0f));\n"
		//"outColor += abs(sin(time + gl_FragCoord.x/4.0f)/5.0f);\n"
		//"if(sqrt(vPos.x*vPos.x + vPos.y*vPos.y) < 0.1f)\n"
		//"{\n"
		//"outColor.a = 1;\n"
		//"}\n"
		//"outColor = vec4(1, 1, 0, 1);\n"
		"vec2 outUV = vUV;\n"
		//"outUV.x += cos(time + outUV.y) + sin(time*1 + outUV.y*0.5)*0.15f;\n"
		"outColor = texture(map, outUV.xy);\n"
		//"outColor.y -= (cos(time + (gl_FragCoord.x+0.75f)/5.0f));\n"
		//"outColor.z += abs(sin(time + (gl_FragCoord.x+0.75f)/10.0f));\n"
		//"outColor += (sin(time + gl_FragCoord.y)/10.0f);\n"
		"}\n";

	Shader s = makeShader(vsource, fsource);
	int loc = 0, tex = 0;
	Framebuffer f = { 0, 800, 600 };

	while (context.step())
	{
		clearFrameBuffer(f);
		loc = 0, tex = 0;
		setUniforms(s, loc, tex, (float)context.getTime(), t_magYel);
		s0_draw(f, s, cube);
	}

	freeGeometry(cube);
	freeShader(s);
	freeTexture(t_magYel);

	context.term();

	return 0;
}