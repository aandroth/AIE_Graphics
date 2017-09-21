
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

	//Texture texture = makeTexture();

	Vertex *verts = new Vertex[4];

	verts[0].position = { 1, 1, 0, 1 }; // LB
	verts[0].color = { 0, 0, 1, 1 };
	verts[0].texCoord = { verts[0].position.x, verts[0].position.y };
	verts[1].position = { 0, 1, 0, 1 };  // RB
	verts[1].color = { 0, 1, 1, 1 };
	verts[1].texCoord = { verts[1].position.x, verts[1].position.y };
	verts[2].position = { 0,  0, 0, 1 };  // RT
	verts[2].color = { 1, 0, 1, 1 };
	verts[2].texCoord = { verts[2].position.x, verts[2].position.y };
	verts[3].position = { 1,  0, 0, 1 }; // LT
	verts[3].color = { 1, 1, 1, 1 };
	verts[3].texCoord = { verts[3].position.x, verts[3].position.y };

	unsigned idxs[6] = { 0, 1, 2, 0, 3, 2 };

	Geometry mirror_geo = MakeGeometry(verts, 4, idxs, (6));
	Geometry g = loadGeometry("../../resources/models/sphere.obj"); //
	Geometry cube_geo = loadGeometry("../../resources/models/cube.obj"); //
	glm::mat4 cubeTraMat = glm::translate(glm::mat4(1.0f), glm::vec3(1, 1, 0.5f));

																  // RGB texture (3 channels), 2x1
	unsigned char pixels[] = { 255, 0, 255, 255, 255, 0 };
	Texture t_magYel = makeTexture(7, 1, 3, pixels, true);

	Texture wall_normal = loadTexture("../../resources/textures/Blue.jpg");
	Texture wall_diffuse = loadTexture("../../resources/textures/Blue.jpg");
	float ss_gloss = 4.0f;

	Shader standard = loadShader("../../resources/shaders/mvpmat.vert",
		"../../resources/shaders/mvpmat.frag");
	Shader mirror_shader = loadShader("../../resources/shaders/mirror.vert", "../../resources/shaders/mirror.frag");

	Shader * shaderPtr = nullptr;
	shaderPtr = &mirror_shader;

	Framebuffer screen = { 0, 800, 600 };

	glm::mat4 *projPtr, *viewPtr;

	// Camera
	glm::mat4 cam_view = glm::lookAt(glm::vec3(0, 3, -4),
									 glm::vec3(0, 0, 0),
									 glm::vec3(0, 1, 0));

	glm::mat4 cam_proj = glm::perspective(145.f, 800.f / 600.f, .01f, 100.f);

	// Mirror
	float mirror_angle = 0, mirror_x = 0, mirror_y = -0.25;
	
	
	// MirrorCam
	glm::mat4 mirror_view = glm::lookAt(glm::vec3(mirror_x+0.5f, 0, mirror_y - 0.5f),
										glm::vec3(0, 0, 1),
										glm::vec3(0, 1, 0));

	glm::mat4 mirror_proj = glm::perspective(300.f, 800.f / 600.f, .01f, 100.f);

	projPtr = &mirror_proj;
	viewPtr = &mirror_view;

	// Model
	glm::mat4 go_model(1.0);

	// Light
	glm::vec3 l_dir = glm::normalize(glm::vec3(1, -1, 1));
	glm::vec4 l_color = glm::vec4(.7f, .6f, .9f, 1);
	float     l_intensity = 1.f;
	glm::vec4 l_ambient = glm::vec4(.2f, .2f, .01f, 1);
	int       l_type = 0;

	Framebuffer fBuffer = makeFrameBuffer(800, 600, 4, true, 3, 1);
	Framebuffer mBuffer = makeFrameBuffer(800, 600, 4, true, 1, 0);
	//mBuffer.targets[0] = t_magYel;
	//Shader fsq_shader = loadShader("../../resources/shaders/quad.vert", "../../resources/shaders/quad.frag");

	//Geometry ss_geo = 
	glm::mat4 rotCamMat(1.0);// = glm::rotate(1.f, glm::vec3(0, 1, 0));

	float angle = 0, up_angle = 0, wall_angle = 0, cutoff = 0;

	while (context.step())
	{
		if (context.getKey('A'))
			angle += 0.01f;
		else if (context.getKey('D'))
			angle -= 0.01f;
		if (context.getKey('W'))
			up_angle += 0.01f;
		else if (context.getKey('S'))
			up_angle -= 0.01f;
		if (context.getKey('Q'))
			cutoff += 0.01f;
		else if (context.getKey('E'))
			cutoff -= 0.01f;
		if (context.getKey('Z'))
			wall_angle += 0.01f;
		else if (context.getKey('X'))
			wall_angle -= 0.01f;

		// Move the mirror around
		if (context.getKey('J'))
			mirror_x -= 0.01f;
		else if (context.getKey('L'))
			mirror_x += 0.01f;
		if (context.getKey('I'))
			mirror_y -= 0.01f;
		else if (context.getKey('K'))
			mirror_y += 0.01f;
		if (context.getKey('U'))
			mirror_angle += 0.01f;
		else if (context.getKey('O'))
			mirror_angle -= 0.01f;
		if (context.getKey('N'))
		{
			shaderPtr = &mirror_shader;
			projPtr = &mirror_proj;
			viewPtr = &mirror_view;
		}
		else if (context.getKey('M'))
		{
			shaderPtr = &standard;
			projPtr = &cam_proj;
			viewPtr = &cam_view;
		}
		rotCamMat = glm::rotate(angle, glm::vec3(0, 1, 0)) * glm::rotate(up_angle, glm::vec3(1, 0, 0));

		float timeVal = (float)context.getTime();
		glm::mat4 rotMat = glm::rotate(timeVal, glm::vec3(0, 1, 0));

		glm::mat4 mirrorTraMat = glm::translate(glm::mat4(1.0f), glm::vec3(mirror_x, 0, mirror_y));
		glm::mat4 mirrorRotMat = glm::rotate(mirror_angle, glm::vec3(0, 1, 0));
		glm::mat4 mirror_model(1.0f);
		//mirror_proj = glm::perspective(145.f, 800.f / 600.f, .01f, 100.f);
		//glm::vec3 reflection_vec = reflection(const glm::vec3 &norm, const glm::vec3 &v)

		//////////////////////////////////////////////////
		// Mirror Frame Buffer Pass
		clearFrameBuffer(mBuffer);
		setFlags(RenderFlag::DEPTH);
		glBindFramebuffer(GL_FRAMEBUFFER, mBuffer.handle);

		int loc = 0, slot = 0;

		mirror_view = glm::lookAt(glm::vec3(mirror_x + 0.5f, 0, mirror_y),
			glm::vec3(mirror_x + 0.5f, 0, mirror_y) + glm::vec3(verts[0].normal.x, verts[0].normal.y, -1),
			glm::vec3(0, 1, 0));

		// Sphere
		loc = 0, slot = 0;
		setUniforms(mirror_shader, loc, slot,
			mirror_proj, mirror_view,
			//go_model, wall_diffuse, wall_specular, wall_normal, wall_gloss, // wall data
			rotCamMat * go_model, wall_diffuse);// , wall_normal, cutoff);// , // wall data
		s0_draw(mBuffer, mirror_shader, g);

		// Cube
		loc = 0, slot = 0;
		setUniforms(mirror_shader, loc, slot,
			mirror_proj, mirror_view,
			//go_model, wall_diffuse, wall_specular, wall_normal, wall_gloss, // wall data
			cubeTraMat * rotCamMat * go_model, wall_diffuse);//, wall_normal, cutoff);// , // wall data
		s0_draw(mBuffer, mirror_shader, cube_geo);


		////////////////////////////////////////////////
		//  Screen pass
		clearFrameBuffer(screen);
		setFlags(RenderFlag::DEPTH);
		glBindFramebuffer(GL_FRAMEBUFFER, screen.handle);
		glViewport(0, 0, 800, 600);

		// Cube
		loc = 0, slot = 0;
		setUniforms(standard, loc, slot,
			//cam_proj, cam_view,
			*projPtr, *viewPtr,
			//go_model, wall_diffuse, wall_specular, wall_normal, wall_gloss, // wall data
			cubeTraMat * rotCamMat * go_model, wall_diffuse, wall_normal, cutoff);// , // wall data
		s0_draw(screen, standard, cube_geo);

		// Sphere
		loc = 0, slot = 0;
		setUniforms(standard, loc, slot,
			//cam_proj, cam_view,
			*projPtr, *viewPtr,
			//go_model, wall_diffuse, wall_specular, wall_normal, wall_gloss, // wall data
			rotCamMat * go_model, wall_diffuse, wall_normal, cutoff);// , // wall data
		s0_draw(screen, standard, g);

		// Mirror
		loc = 0, slot = 0;
		setUniforms(standard, loc, slot,
			//cam_proj, cam_view,
			*projPtr, *viewPtr,
			//go_model, wall_diffuse, wall_specular, wall_normal, wall_gloss, // wall data
			mirrorTraMat * mirrorRotMat * mirror_model, mBuffer.targets[0], wall_normal, cutoff);// , // wall data
		s0_draw(screen, standard, mirror_geo);
	}

	context.term();
	delete[] verts;

	return 0;
}