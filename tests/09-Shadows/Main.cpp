
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
	// Floor
	Vertex vquad[] = {
		{ { -1,-1,0,1 },{},{ 0,0 },{ 0,0,1,0 } },
		{ { 1,-1,0,1 },{},{ 1,0 },{ 0,0,1,0 } },
		{ { 1, 1,0,1 },{},{ 1,1 },{ 0,0,1,0 } },
		{ { -1, 1,0,1 },{},{ 0,1 },{ 0,0,1,0 } } };
	unsigned quadidx[] = { 0,1,3, 1,2,3 };

	Geometry geo_floor = MakeGeometry(vquad, 4, quadidx, 6);
	glm::mat4 model_floor = glm::rotate(glm::radians(90.f), glm::vec3(-1, 0, 0))*glm::scale(glm::vec3(5, 5, 1));

	Geometry geo_cube = loadGeometry("../../resources/models/cube.obj");
	glm::mat4 model_cube = glm::translate(glm::vec3(2, 1, -1));

	Geometry geo_spear = loadGeometry("../../resources/models/soulspear.obj");
	glm::mat4 model_spear;
	Texture diffuse = loadTexture("../../resources/textures/soulspear_diffuse.tga");

	// Light
	glm::vec3 light_dir = glm::normalize(glm::vec3(1, -1, -1));
	glm::mat4 light_proj = glm::ortho<float>(-10, 10, -10, 10, -10, 10);
	glm::mat4 light_view = glm::lookAt(-light_dir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	// Shaders
	Shader shdr_shadow = loadShader("../../resources/shadow.vert", "../../resources/shadow.frag");
	Shader shdr_direct = loadShader("../../resources/direct.vert", "../../resources/direct.frag");
	// Buffers
	Framebuffer fb_shadow = makeFrameBuffer(2048, 2048, 0, true, 0, 0);
	Framebuffer screen = { 0, 1280, 720 };

	int loc = 0, tex = 0;

	glm::mat4 cam_view = glm::lookAt(glm::vec3(0, 2, 5), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	glm::mat4 cam_proj = glm::perspective(45.f, 1280.f / 720.f, 1.f, 10.f);
	glm::mat4 go_model; //identity matrix

	while (context.step())
	{
		// Shadow Pass
		clearFrameBuffer(fb_shadow, false, true);
		setFlags(RenderFlag::DEPTH);

		float timeVal = (float)context.getTime();
		float speed = 1;
		glm::mat4 rotMat = glm::rotate(timeVal * speed, glm::vec3(0, 1, 0));

		loc = tex = 0;
		setUniforms(shdr_shadow, loc, tex, light_proj, light_view, model_floor);// , diffuse, light_dir, glm::vec4(1.f, 1.f, 1.f, 1.f));
		s0_draw(fb_shadow, shdr_shadow, geo_floor);

		loc = tex = 0;
		setUniforms(shdr_shadow, loc, tex, light_proj, light_view, model_spear);// , diffuse, light_dir, glm::vec4(1.f, 1.f, 1.f, 1.f));
		s0_draw(fb_shadow, shdr_shadow, geo_spear);

		// Light Pass		
		setFlags(RenderFlag::DEPTH);
		clearFrameBuffer(screen);

		loc = tex = 0;
		setUniforms(shdr_direct, loc, tex,
			cam_proj, cam_view,               // Camera Data
			model_floor,                      // Geometry Data
			light_proj, light_view, light_dir,// Light Data
			fb_shadow.depthTarget,            // Shadow Map
			diffuse);				          // Texture
		s0_draw(screen, shdr_direct, geo_floor);

		loc = tex = 0;
		setUniforms(shdr_direct, loc, tex,
			cam_proj, cam_view,               // Camera Data
			rotMat * model_spear,                      // Geometry Data
			light_proj, light_view, light_dir,// Light Data
			fb_shadow.depthTarget,            // Shadow Map
			diffuse);				          // Texture
		s0_draw(screen, shdr_direct, geo_spear);
	}

	freeGeometry(geo_floor);
	freeGeometry(geo_cube);
	freeGeometry(geo_spear);
	freeShader(shdr_shadow);
	freeTexture(diffuse);

	context.term();

	return 0;
}