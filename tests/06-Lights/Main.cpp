
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

	glm::vec3 dl_dir = glm::normalize(glm::vec3(0, 1, 0));
	glm::vec4 dl_col = glm::vec4(.7f, .6f, .2f, 1.f);

	Shader s = loadShader("../../resources/shaders/lambert.vert", "../../resources/shaders/lambert.frag");
	int loc = 0, tex = 0;
	Framebuffer f = { 0, 800, 600 };
	float speed = 1;

	glm::mat4 cam_view = glm::lookAt(glm::vec3(0, 3, -4), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	glm::mat4 cam_proj = glm::perspective(1.f, 800.f / 600.f, 0.01f, 100.f);
	glm::mat4 go_model; //identity matrix

	// RGB texture (3 channels), 2x1
	unsigned char pixels[] = { 255, 0, 255,
		255, 255, 0 };

	Texture t_magYel = makeTexture(7, 1, 3, pixels, true);

	while (context.step())
	{
		clearFrameBuffer(f);
		setFlags(RenderFlag::DEPTH);

		float timeVal = (float)context.getTime();
		//glm::mat4 rotMat = glm::rotate(timeVal * speed, glm::vec3(1, 1.3, 0.2));
		glm::mat4 rotMat = glm::rotate(timeVal * speed, glm::vec3(0, 1, 0));
		dl_dir.x = sin(timeVal * speed);
		dl_dir.z = -cos(timeVal * speed);
		//dl_col.x = sin(timeVal * speed);
		//dl_col.z = -cos(timeVal * speed);

		loc = 0, tex = 0;
		setUniforms(s, loc, tex, cam_proj, cam_view, rotMat * go_model, t_magYel, dl_dir, dl_col);
		s0_draw(f, s, spear);
	}

	freeGeometry(spear);
	freeShader(s);
	freeTexture(diffuse);

	context.term();

	return 0;
}