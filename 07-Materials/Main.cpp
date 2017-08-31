
#include "graphics\Context.h"
#include "graphics\draw.h"
#include "graphics\RenderObjects.h"
#include "graphics\Vertex.h"
#include "graphics\load.h"
#include "graphics\glm\ext.hpp"


int main()
{
	Context context;
	context.init();

	Vertex *verts = new Vertex[4];

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

	unsigned idxs[6] = { 0, 1, 2, 0, 3, 2 };

	Geometry g = MakeGeometry(verts, 4,
		idxs, (6));

	// RGB texture (3 channels), 2x1
	unsigned char pixels[] = { 255, 0, 255,
		255, 255, 0 };

	Texture wall_normal = loadTexture("../../resources/carving.jpg");
	Texture wall_diffuse = loadTexture("../../resources/duck.jpg");

	Shader standard = loadShader("../../resources/shaders/standard.vert",
								 "../../resources/shaders/standard.frag");

	Framebuffer screen = { 0, 800, 600 };

	// Camera
	glm::vec4 cam_view = glm::lookAt(glm::vec3(0, 3, -4),
									glm::vec3(0, 0, 0),
									glm::vec3(0, 1, 0));

	glm::vec4 cam_proj = glm::perspective(45.f, 800.f / 600.f, .01f, 100.f);

	// Model
	glm::mat4 go_model;

	// Light
	glm::vec3 light_direction = glm::normalize(glm::vec3(1, -1, 1));

	while (context.step())
	{
		float time = context.getTime();

		//glm::mat4 model = glm::rotate(time, glm::vec3(0, 1, 0));
		go_model = glm::rotate(time, glm::vec3(0, 1, 0)) * glm::rotate(glm::radians(90.f), glm::vec3(-1, 0, 0)) * glm::scale(glm::vec3(2, 2, 1));

		clearFrameBuffer(screen);
		setFlags(RenderFlag::DEPTH);

		int loc = 0, slot = 0;

		setUniforms(standard, loc, slot,
			cam_proj, cam_view,
			go_model, wall_normal, wall_diffuse,
			light_direction);

		s0_draw(screen, standard, g);
	}

	context.term();
	delete[] verts;

	return 0;
}