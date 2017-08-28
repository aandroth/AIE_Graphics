
#include "graphics\Context.h"
#include "graphics\Vertex.h"
#include "graphics\draw.h"
#include <math.h>
# define M_PI           3.14159265358979323846
#include <vector>
using std::vector;
#include <iostream>
using std::cout;

int main()
{
	Context context;
	context.init(800, 600, "GRAPHICS!!!!");

	//// Declare triangle verts
	//glm::vec4 col_triangle = { 1, 1, 0, 1 };
	//Vertex  *verts_triangle = new Vertex[3];
	//unsigned *idxs_triangle = new unsigned[3];

	//// Put in triangle values
	//verts_triangle[0].position = glm::vec4(-0.5f, -0.5f, 0, 1);
	//verts_triangle[1].position = glm::vec4(0.5f, -0.5f, 0, 1);
	//verts_triangle[2].position = glm::vec4(0, 0.5f, 0, 1);
	//for (int ii = 0; ii < 3; ++ii)
	//{
	//	verts_triangle[ii].color = col_triangle;
	//	idxs_triangle[ii] = ii;
	//}

	// Declare ngon side amount
	size_t ngon_side_count = 49, ngon_idxs_count = ngon_side_count * 3;
	float ngon_radius = 0.5;
	glm::vec4 col_ngon = { 0.7f, 0.7f, 0.7f, 1 };
	Vertex  *verts_ngon = new Vertex[ngon_side_count + 1];
	unsigned *idxs_ngon = new unsigned[ngon_idxs_count];

	// Put in the values of the vert and idxs
	verts_ngon[0].position = glm::vec4(0, 0, 0, 1);
	verts_ngon[0].color = col_ngon;
	for (int ii = 1; ii < ngon_side_count+1; ++ii)
	{
		float ratio = ((float)ii / (float)ngon_side_count);
		float angle = (360.f*ratio);
		float xPos = cos((angle)* M_PI/180);
		float yPos = sin((angle)* M_PI / 180);
		verts_ngon[ii].position = glm::vec4(xPos* ngon_radius, yPos * ngon_radius, 0, 1);
		if (ratio < 0.33)
			verts_ngon[ii].color = glm::vec4(0, (ratio * 3), 1 - (ratio*3), 1);// col_ngon;
		else if(ratio < 0.66)
			verts_ngon[ii].color = glm::vec4(((ratio - 0.33f) * 3), 1 - ((ratio - 0.33f)*3), 0, 1);// col_ngon;
		else//(ratio < 1)
			verts_ngon[ii].color = glm::vec4(1 - ((ratio - 0.66f) * 3), 0, ((ratio - 0.66f) * 3), 1);// col_ngon;
	}
	
	int kk = 0;
	for (int ii = 1; ii < ngon_side_count; ++ii)
	{
		idxs_ngon[kk++] = 0;
		idxs_ngon[kk++] = ii;
		idxs_ngon[kk++] = ii + 1;
	}
	idxs_ngon[kk++] = 0;
	idxs_ngon[kk++] = ngon_side_count;
	idxs_ngon[kk++] = 1;

	//// Declare checkerboard width and height
	//size_t checkerW = 4, checkerH = 4;
	//size_t vert_checkerboard_size = (checkerW + 1) * (checkerH + 1);
	//size_t idxs_checkerboard_size = 6 * checkerW * checkerH;
	//int checker_offset = 0.1;
	//glm::vec4 col_checkerboard_0 = { 0, 1, 0, 1 }, col_checkerboard_1 = { 0, 0, 1, 1 };
	//Vertex  *verts_checkerboard = new Vertex[(checkerW + 1) * (checkerH + 1)];
	//unsigned *idxs_checkerboard = new unsigned[idxs_checkerboard_size];
	//
	//// Create the vert positions for the checkerboard geometry
	//for (int ii = 0; ii < (checkerH + 1); ++ii)
	//{
	//	for (int jj = 0; jj < (checkerW + 1); ++jj)
	//	{
	//		verts_checkerboard[ii*(checkerW+1) + jj].position = { 0.1f * jj, 0.1f*ii, 0, 1 };
	//		verts_checkerboard[ii*(checkerW + 1) + jj].color = (jj % 2 != ii % 2) ? col_checkerboard_0 : col_checkerboard_1;
	//	}
	//}

	//int kk = 0;
	//for (int ii = 0; ii < (checkerW); ++ii)
	//{
	//	for (int jj = 0; jj < (checkerH); ++jj)
	//	{

	//		//idxs[ii*jj + jj] = { LB, RB, RT, LB, LT, RT };
	//		//idxs[ii*jj + jj] = { 0, 1, 2, 0, 3, 2 };
	//		idxs_checkerboard[kk++] = ((ii + 1)*(checkerW + 1) + jj);     // LB
	//		idxs_checkerboard[kk++] = ((ii + 1)*(checkerW + 1) + jj+1);   // RB
	//		idxs_checkerboard[kk++] = ((ii + 0)*(checkerW + 1) + jj+1);   // RT
	//		idxs_checkerboard[kk++] = ((ii + 1)*(checkerW + 1) + jj);     // LB
	//		idxs_checkerboard[kk++] = ((ii + 0)*(checkerW + 1) + jj);     // LT
	//		idxs_checkerboard[kk++] = ((ii + 0)*(checkerW + 1) + jj + 1); // RT
	//	}
	//}
	

	//verts[0].position = { -0.5f, -0.5f, 0, 1 }; // LB
	//verts[0].color = { 0, 0, 1, 1 };
	//verts[1].position = { 0.5f, -0.5f, 0, 1 };  // RB
	//verts[1].color = { 0, 1, 1, 1 };
	//verts[2].position = { 0.5f,  0.5f, 0, 1 };  // RT
	//verts[2].color = { 1, 0, 1, 1 };
	//verts[3].position = { -0.5f,  0.5f, 0, 1 }; // LT
	//verts[3].color = { 1, 0, 1, 1 };


	//Geometry geo_triangle = MakeGeometry(verts_triangle, 3,
	//	idxs_triangle, 3);

	Geometry geo_ngon = MakeGeometry(verts_ngon, ngon_side_count+1,
		idxs_ngon, ngon_idxs_count);

	//Geometry geo_checkerboard = MakeGeometry(verts_checkerboard, vert_checkerboard_size,
	//	idxs_checkerboard, idxs_checkerboard_size);

	const char *vsource = "#version 430\n"
		"layout(location = 0) in vec4 position; \n"
		"layout(location = 1) in vec4 color; \n"
		"layout(location = 2) in mat4 vert; \n"
		"flat out vec4 vCol; \n"
		"out vec4 vPos; \n"
		"void main()\n{\n"
		"vCol = color;\n"
		"vPos = position;\n"
		"gl_Position = vPos;\n}\n";

	const char *fsource = "#version 430\n"
		"flat in vec4 vCol; \n"
		"in vec4 vPos; \n"
		"out vec4 outColor; \n"
		"void main()\n{\n"
		"outColor = vCol;\n}\n";

	Shader s = makeShader(vsource, fsource);

	Framebuffer f = {0, 800, 600};

	while (context.step())
	{
		s0_draw(f, s, geo_ngon);
	}

	freeGeometry(geo_ngon);
	freeShader(s);

	context.term();

	return 0;
}
//
//void CreateTriangleToVertsAndIdxs(Vertex * verts, unsigned * idxs)
//{
//
//}