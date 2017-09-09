
#include "..\include\graphics\GameObjects.h"

Geometry MakeNGon(int sideCount) {

	// Declare ngon side amount
	size_t ngon_side_count = 49;
	size_t ngon_idxs_count = sideCount * 3;
	float ngon_radius = 0.5;
	glm::vec4 col_ngon = { 0.7f, 0.7f, 0.7f, 1 };
	Vertex  *verts_ngon = new Vertex[ngon_side_count + 1];
	unsigned *idxs_ngon = new unsigned[ngon_idxs_count];

	// Put in the values of the vert and idxs
	verts_ngon[0].position = glm::vec4(0, 0, 0, 1);
	verts_ngon[0].color = col_ngon;
	for (int ii = 1; ii < ngon_side_count + 1; ++ii)
	{
		float ratio = ((float)ii / (float)ngon_side_count);
		float angle = (360.f*ratio);
		float xPos = cos((angle)* M_PI / 180);
		float yPos = sin((angle)* M_PI / 180);
		verts_ngon[ii].position = glm::vec4(xPos* ngon_radius, yPos * ngon_radius, 0, 1);
		if (ratio < 0.33)
			verts_ngon[ii].color = glm::vec4(0, (ratio * 3), 1 - (ratio * 3), 1);// col_ngon;
		else if (ratio < 0.66)
			verts_ngon[ii].color = glm::vec4(((ratio - 0.33f) * 3), 1 - ((ratio - 0.33f) * 3), 0, 1);// col_ngon;
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

	Geometry geo_ngon = MakeGeometry(verts_ngon, ngon_side_count+1,
		idxs_ngon, ngon_idxs_count);

	return geo_ngon;
}