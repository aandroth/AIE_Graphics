
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
	glm::vec3 light_dir = glm::normalize(glm::vec3(1, -1, 1));
	glm::mat4 light_proj = glm::ortho<float>(-10, 10, -10, 10, -10, 10);
	glm::mat4 light_view = glm::lookAt(-light_dir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	// Shaders
	//Shader shdr_shadow = loadShader("../../resources/shaders/shadow.vert", "../../resources/shaders/shadow.frag");
	Shader shdr_shadow = loadShader("../../resources/mvplite.vert", "../../resources/mvplite.frag");
	//Shader shdr_direct = loadShader("../../resources/shaders/direct.vert", "../../resources/shaders/direct.frag");
	// Buffers
	Framebuffer fb_shadow = makeFrameBuffer(2048, 2048, 0, true, 0, 0);
	Framebuffer screen = { 0, 1280, 720 };
	Framebuffer f = { 0, 800, 600 };

	int loc = 0, tex = 0;

	glm::mat4 cam_view = glm::lookAt(glm::vec3(0, 2, 5), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	glm::mat4 cam_proj = glm::perspective(45.f, 1280.f / 720.f, 1.f, 10.f);
	glm::mat4 go_model; //identity matrix
	//glm::mat4 cam_view = glm::lookAt(glm::vec3(0, 3, -4), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	//glm::mat4 cam_proj = glm::perspective(1.f, 800.f / 600.f, 0.01f, 100.f);
	//glm::mat4 go_model(1.0); //identity matrix

	while (context.step())
	{
		// Shadow Pass
		//clearFrameBuffer(fb_shadow, false, true);
		clearFrameBuffer(screen);
		setFlags(RenderFlag::DEPTH);

		loc = tex = 0;
		//setUniforms(shdr_shadow, loc, tex, cam_proj, cam_view, go_model, diffuse, light_dir, glm::vec4(1.f, 1.f, 1.f, 1.f));
		setUniforms(shdr_shadow, loc, tex, cam_proj, cam_view, go_model, diffuse);
		//s0_draw(screen, shdr_shadow, geo_spear);
		s0_draw(screen, shdr_shadow, geo_spear);
		loc = tex = 0;
		
		s0_draw(screen, shdr_shadow, geo_floor);

		// Light Pass		
		//setFlags(RenderFlag::DEPTH);
		//clearFrameBuffer(screen);

		//loc = tex = 0;
		//setUniforms(shdr_direct, loc, tex,
		//	cam_proj, cam_view,     // Camera Data
		//	model_floor,            // Geometry Data
		//	light_proj, light_view, // Light Data
		//	fb_shadow.depthTarget); // Shadow Map
		//s0_draw(screen, shdr_direct, geo_floor);
	}

	freeGeometry(geo_floor);
	freeGeometry(geo_cube);
	freeGeometry(geo_spear);
	freeShader(shdr_shadow);
	freeTexture(diffuse);

	context.term();

	return 0;
}