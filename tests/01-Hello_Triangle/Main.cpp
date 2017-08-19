
#include "..\..\include\graphics\Context.h"
#include "graphics\Vertex.h"
#include "graphics\RenderObjects.h"
#include "graphics\draw.h"
#include <vector>
using std::vector;

int main()
{
	Context context;
	context.init(800, 600, "GRAPHICS!!!!");

	// Declare checkerboard width and height
	const int checkerW = 4, checkerH = 4;
	int checker_offset = 0.1;
	glm::vec4 c0 = { 0, 1, 0, 1 }, c1 = {0, 0, 1, 1};
	

	Vertex *verts = new Vertex[(checkerW+1) * (checkerH+1)];
	unsigned idxs[6 * checkerW * checkerH];

	vector<unsigned> idx_vec;
	
	for (int ii = 0; ii < (checkerH + 1); ++ii)
	{
		for (int jj = 0; jj < (checkerW + 1); ++jj)
		{
			verts[ii*(checkerW+1) + jj].position = { 0.1f * jj, 0.1f*ii, 0, 1 };
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
			idx_vec.push_back((ii + 1)*(checkerW + 1) + jj+1);   // RB
			idx_vec.push_back((ii + 0)*(checkerW + 1) + jj+1);   // RT
			idx_vec.push_back((ii + 1)*(checkerW + 1) + jj);     // LB
			idx_vec.push_back((ii + 0)*(checkerW + 1) + jj);     // LT
			idx_vec.push_back((ii + 0)*(checkerW + 1) + jj + 1); // RT
		}
	}

	for (int ii = 0; ii < idx_vec.size(); ++ii)
	{
		idxs[ii] = idx_vec[ii];
	}
	

	//verts[0].position = { -0.5f, -0.5f, 0, 1 }; // LB
	//verts[0].color = { 0, 0, 1, 1 };
	//verts[1].position = { 0.5f, -0.5f, 0, 1 };  // RB
	//verts[1].color = { 0, 1, 1, 1 };
	//verts[2].position = { 0.5f,  0.5f, 0, 1 };  // RT
	//verts[2].color = { 1, 0, 1, 1 };
	//verts[3].position = { -0.5f,  0.5f, 0, 1 }; // LT
	//verts[3].color = { 1, 0, 1, 1 };


	Geometry g = MakeGeometry(verts, (checkerW + 1) * (checkerH + 1), 
		idxs, (6 * checkerW * checkerH));

	const char *vsource = "#version 450\n"
		"layout(location = 0) in vec4 position; \n"
		"layout(location = 1) in vec4 color; \n"
		"out vec4 vCol; \n"
		"out vec4 vPos; \n"
		"void main()\n{\nvCol = color;"
		"\nvPos = position;"
		"\ngl_Position = position;\n}\n";

	const char *fsource = "#version 450\n"
		"flat in vec4 vCol; \n"
		"in vec4 vPos; \n"
		"out vec4 outColor; \n"
		"void main()\n{\noutColor = vCol;\n}\n";

	Shader s = makeShader(vsource, fsource);

	Framebuffer f = {0, 800, 600};


	while (context.step())
	{
		s0_draw(f, s, g);
	}

	freeGeometry(g);
	freeShader(s);

	context.term();

	return 0;
}