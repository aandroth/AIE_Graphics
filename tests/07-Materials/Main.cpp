
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

	Geometry g = loadGeometry("../../resources/models/cube.obj"); //MakeGeometry(verts, 4, idxs, (6));

	// RGB texture (3 channels), 2x1
	unsigned char pixels[] = { 255, 0, 255,
		255, 255, 0 };

	Texture wall_normal = loadTexture("../../resources/textures/carving.jpg");
	Texture wall_diffuse = loadTexture("../../resources/textures/Blue.jpg");
	float ss_gloss = 4.0f;

	Shader standard = loadShader("../../resources/shaders/mvp_mat.vert",
								 "../../resources/shaders/mvp_mat.frag");

	Framebuffer screen = { 0, 800, 600 };

	// Camera
	glm::mat4 cam_view = glm::lookAt(glm::vec3(0, 3, -4),
									glm::vec3(0, 0, 0),
									glm::vec3(0, 1, 0));

	glm::mat4 cam_proj = glm::perspective(145.f, 800.f / 600.f, .01f, 100.f);

	// Model
	glm::mat4 go_model(1.0);

	// Light
	glm::vec3 l_dir           = glm::normalize(glm::vec3(1, -1, 1));
	glm::vec4 l_color         = glm::vec4(.7f, .6f, .9f, 1);
	float     l_intensity     = 1.f;
	glm::vec4 l_ambient       = glm::vec4(.2f, .2f, .01f, 1);
	int       l_type          = 0;

	Framebuffer fBuffer = makeFrameBuffer(800, 600, 4, true, 3, 1);
	//Shader fsq_shader = loadShader("../../resources/shaders/quad.vert", "../../resources/shaders/quad.frag");

	//Geometry ss_geo = 

	while (context.step())
	{

		float timeVal = (float)context.getTime();
		glm::mat4 rotMat = glm::rotate(timeVal, glm::vec3(0, 1, 0));

		//glm::mat4 model = glm::rotate(time, glm::vec3(0, 1, 0));
		//go_model = glm::rotate(time, glm::vec3(0, 1, 0)) * glm::rotate(glm::radians(90.f), glm::vec3(-1, 0, 0)) * glm::scale(glm::vec3(2, 2, 1));

		//////////////////////////////////////////////////
		// Frame Buffer Pass
		clearFrameBuffer(screen);
		setFlags(RenderFlag::DEPTH);

		int loc = 0, slot = 0;

		setUniforms(standard, loc, slot,
			cam_proj, cam_view,
			//go_model, wall_diffuse, wall_specular, wall_normal, wall_gloss, // wall data
			rotMat * go_model, wall_diffuse, wall_normal);// , // wall data
			//l_dir, l_color, l_intensity, l_ambient, l_type);				// light data

		s0_draw(screen, standard, g);

		////////////////////////////////////////////////
		//  Screen pass
		//clearFrameBuffer(screen);
		//loc = 0; slot = 0;
		//setUniforms(fsq_shader, loc, slot, fBuffer.targets[0]);
		//	s0_draw(screen, fsq_shader, g);
	}

	context.term();
	delete[] verts;

	return 0;
}