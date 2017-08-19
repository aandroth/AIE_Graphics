
#include "..\..\include\graphics\Context.h"
#include "graphics\Vertex.h"
#include "graphics\RenderObjects.h"
#include "graphics\draw.h"
#include <vector>
using std::vector;
#include <iostream>
using std::cout;

int main()
{
	Context context;
	context.init(800, 600, "GRAPHICS!!!!");

	// Declare checkerboard width and height
	const int checkerW = 4, checkerH = 4;
	int checker_offset = 0.1;
	glm::vec4 c0 = { 0, 1, 0, 1 }, c1 = { 0, 0, 1, 1 };

	Vertex *verts = new Vertex[4];

	vector<unsigned> idx_vec;
	/*
	for (int ii = 0; ii < (checkerH + 1); ++ii)
	{
		for (int jj = 0; jj < (checkerW + 1); ++jj)
		{
			verts[ii*(checkerW + 1) + jj].position = { 0.1f * jj, 0.1f*ii, 0, 1 };
			verts[ii*(checkerW + 1) + jj].color = (jj % 2 != ii % 2) ? c0 : c1;
		}
	}

	for (int ii = 0; ii < (checkerW); ++ii)
	{
		for (int jj = 0; jj < (checkerH); ++jj)
		{

			//idxs[ii*jj + jj] = { LB, RB, RT, LB, LT, RT };
			//idxs[ii*jj + jj] = { 0, 1, 2, 0, 3, 2 };
			idx_vec.push_back((ii + 1)*(checkerW + 1) + jj);     // LB
			idx_vec.push_back((ii + 1)*(checkerW + 1) + jj + 1);   // RB
			idx_vec.push_back((ii + 0)*(checkerW + 1) + jj + 1);   // RT
			idx_vec.push_back((ii + 1)*(checkerW + 1) + jj);     // LB
			idx_vec.push_back((ii + 0)*(checkerW + 1) + jj);     // LT
			idx_vec.push_back((ii + 0)*(checkerW + 1) + jj + 1); // RT
		}
	}

	for (int ii = 0; ii < idx_vec.size(); ++ii)
	{
		idxs[ii] = idx_vec[ii];
	}

*/
	verts[0].position = { -0.5f, -0.5f, 0, 1 }; // LB
	verts[0].color = { 0, 0, 1, 1 };
	verts[0].texCoord = { verts[0].position.x, verts[0].position.y };
	verts[1].position = { 0.5f, -0.5f, 0, 1 };  // RB
	verts[1].color = { 0, 1, 1, 1 };
	verts[1].texCoord = { verts[1].position.x, verts[1].position.y };
	verts[2].position = { 0.5f,  0.5f, 0, 1 };  // RT
	verts[2].color = { 1, 0, 1, 1 };
	verts[2].texCoord = { verts[2].position.x, verts[2].position.y };
	verts[3].position = { -0.5f,  0.5f, 0, 1 }; // LT
	verts[3].color = { 1, 1, 1, 1 };
	verts[3].texCoord = { verts[3].position.x, verts[3].position.y };

	unsigned idxs[6] = {0, 1, 2, 0, 3, 2};
	
	Geometry g = MakeGeometry(verts, 4,
							idxs, (6));

	// RGB texture (3 channels), 2x1
	unsigned char pixels[] = { 255, 0, 255,
								255, 255, 0 };

	Texture t_magYel = makeTexture(7, 1, 3, pixels);
	Texture t_mask = makeTexture(20, 1, 1, pixels);

	const char *vsource = "#version 450\n"
		"layout(location = 0) in vec4 position; \n"
		"layout(location = 1) in vec4 color; \n"
		"layout(location = 0) uniform float time; \n"
		"out vec4 vCol; \n"
		"out vec2 vUV; \n"
		"out vec4 vPos; \n"

		"void main()\n{\n"
			"vCol = color;\n"
			"vPos = position;\n"
			"gl_Position = position;\n"
			//"gl_Position.x += sin(time);\n"
			//"gl_Position.y += sin(time + gl_Position.x + gl_Position.y)/5.0f;\n"

			//"if(sin(sin(position.x)) < 0.1f)\n"
			//"{\n"
			//"vCol.x = 1.0f;\n"
			//"vCol.y = 0.0f;\n"
			//"vCol.z = 0.0f;\n"
			//"}\n"

			"vUV = position.xy;\n"
		"}\n";

	const char *fsource = "#version 450\n"
		"in vec4 vCol; \n"
		"in vec4 vPos; \n"
		"in vec2 vUV; \n"
		"out vec4 outColor; \n"
		"layout(location = 0) uniform float time; \n"
		"layout(location = 4) uniform sampler2D map; \n"
		//"layout(location = 5) uniform sampler2D mask; \n"

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
		"outUV.x += cos(time + outUV.y) + sin(time*1 + outUV.y*0.5)*0.15f;\n"
		"outColor = texture(map, outUV.xy);\n"
		//"outColor.y -= (cos(time + (gl_FragCoord.x+0.75f)/5.0f));\n"
		//"outColor.z += abs(sin(time + (gl_FragCoord.x+0.75f)/10.0f));\n"
		//"outColor += (sin(time + gl_FragCoord.y)/10.0f);\n"
		"}\n";

	Shader s = makeShader(vsource, fsource);

	Framebuffer f = { 0, 800, 600 };

	while (context.step())
	{
		clearFrameBuffer(f);

		setUniform(s, 0, (float)context.getTime());
		setUniform(s, 4, t_magYel, 0);
		s0_draw(f, s, g);
	}

	freeGeometry(g);
	freeShader(s);
	freeTexture(t_magYel);

	context.term();

	return 0;
}