
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

	Geometry spear = loadGeometry("../../resources/models/soulspear.obj");
	Texture diffuse = loadTexture("../../resources/textures/soulspear_diffuse.tga");
	Shader s = loadShader("../../resources/mvplite.vert", "../../resources/mvplite.frag");

	int loc = 0, tex = 0;
	Framebuffer f = { 0, 800, 600 };
	float speed = 1;

	glm::mat4 cam_view = glm::lookAt(glm::vec3(0, 3, -4), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	glm::mat4 cam_proj = glm::perspective(1.f, 800.f/600.f, 0.01f, 100.f);
	//glm::mat4 go_model(1.0); //identity matrix
	glm::mat4 go_model(1.0); //identity matrix
	//go_model[2][2] = 20;
	//go_model *= glm::mat4(glm::vec4(0.5, 0.5, 0.5, 1), glm::vec4(0.5, 0.5, 4, 1), glm::vec4(0.5, 0.5, 0.5, 1), glm::vec4(0.5, 0.5, 0.5, 1));

	while (context.step())
	{
		clearFrameBuffer(f);
		setFlags(RenderFlag::DEPTH);

		float timeVal = (float)context.getTime();
		//glm::mat4 rotMat = glm::rotate(timeVal * speed, glm::vec3(1, 1.3, 0.2));
		glm::mat4 rotMat = glm::rotate(timeVal * speed, glm::vec3(0, 1, 0));

		loc = 0, tex = 0;
		//setUniforms(s, loc, tex, cam_proj, cam_view, go_model * rotMat, diffuse);
		setUniforms(s, loc, tex, cam_proj, cam_view, rotMat * go_model, diffuse);
		s0_draw(f, s, spear);
	}

	freeGeometry(spear);
	freeShader(s);
	freeTexture(diffuse);

	context.term();

	return 0;
}