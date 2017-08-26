
#include "graphics\Context.h"
#include "graphics\Vertex.h"
#include "graphics\draw.h"
#include "graphics\load.h"
#include <vector>
using std::vector;
#include "graphics\glm\ext.hpp"

int main()
{
	Context context;
	context.init(800, 600, "GRAPHICS!!!!");

	Geometry cube = loadGeometry("../../resources/models/cube.obj");
	Texture t_blue = loadTexture("../../resources/textures/Blue.jpg");

	Shader s = loadShader("../../resources/cube.vert", "../../resources/cube.frag");
	int loc = 0, tex = 0;
	Framebuffer f = { 0, 800, 600 };
	float speed = 1;

	Vertex vquad[] = {
		{ { -1,-1,0,1 },{},{ 0,0 } },
		{ { 1,-1,0,1 },{},{ 1,0 } },
		{ { 1,1,0,1 },{},{ 1,1 } },
		{ { -1,1,0,1 },{},{ 0,1 } }
	};

	unsigned quadidx[] = { 0,1,3, 1,2,3 };
	Geometry quad = MakeGeometry(vquad, 4, quadidx, 6);

	while (context.step())
	{
		clearFrameBuffer(f);
		setFlags(RenderFlag::DEPTH);

		float timeVal = (float)context.getTime();
		//glm::mat4 rotMat = glm::rotate(timeVal * speed, glm::vec3(1, 1.3, 0.2));
		glm::mat4 rotMat = glm::rotate(timeVal * speed, glm::vec3(0, 1, 0));

		loc = 0, tex = 0;
		setUniforms(s, loc, tex, rotMat, timeVal, t_blue);
		s0_draw(f, s, cube);
	}

	freeGeometry(cube);
	freeShader(s);
	freeTexture(t_blue);

	context.term();

	return 0;
}