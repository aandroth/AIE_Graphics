
#include "graphics\Context.h"
#include "graphics\draw.h"
#include "graphics\RenderObjects.h"
#include "graphics\Vertex.h"
#include "graphics\load.h"
#include "graphics\glm\ext.hpp"

glm::vec3 getNormal(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2);

int main()
{
	Context context;
	context.init();

	//Texture texture = makeTexture();

	Vertex *verts = new Vertex[4];
	verts[0].position = { 0, 0, 0, 1 }; // LB
	verts[1].position = { 1, 0, 0, 1 };  // RB
	verts[2].position = { 1,  1, 0, 1 };  // RT
	verts[3].position = { 0,  1, 0, 1 }; // LT
	verts[0].texCoord = { verts[0].position.x, verts[0].position.y };
	verts[1].texCoord = { verts[1].position.x, verts[1].position.y };
	verts[2].texCoord = { verts[2].position.x, verts[2].position.y };
	verts[3].texCoord = { verts[3].position.x, verts[3].position.y };
	verts[0].color = { 0, 0, 1, 1 };
	verts[1].color = { 0, 1, 1, 1 };
	verts[2].color = { 1, 0, 1, 1 };
	verts[3].color = { 1, 1, 1, 1 };

	Vertex *wall_verts = new Vertex[4];
	wall_verts[0].position = { 0, 0, 0, 1 }; // LB
	wall_verts[1].position = { 1, 0, 0, 1 };  // RB
	wall_verts[2].position = { 1,  1, 0, 1 };  // RT
	wall_verts[3].position = { 0,  1,0, 1 }; // LT
	wall_verts[0].texCoord = { wall_verts[0].position.x, wall_verts[0].position.y };
	wall_verts[1].texCoord = { wall_verts[1].position.x, wall_verts[1].position.y };
	wall_verts[2].texCoord = { wall_verts[2].position.x, wall_verts[2].position.y };
	wall_verts[3].texCoord = { wall_verts[3].position.x, wall_verts[3].position.y };
	wall_verts[0].color = { 0, 0, 1, 1 };
	wall_verts[1].color = { 0, 1, 1, 1 };
	wall_verts[2].color = { 1, 0, 1, 1 };
	wall_verts[3].color = { 1, 1, 1, 1 };

	unsigned idxs[6] = { 0, 1, 2, 0, 3, 2 };

	Geometry goList[4];
	goList[0] = MakeGeometry(verts, 4, idxs, (6));
	SolveTangents(verts, 4, idxs, 6);
	goList[1] = loadGeometry("../../resources/models/sphere.obj"); //
	goList[2] = loadGeometry("../../resources/models/cube.obj"); //
	goList[3] = MakeGeometry(wall_verts, 4, idxs, (6));
	SolveTangents(wall_verts, 4, idxs, 6);
	glm::mat4 cubeTraMat = glm::translate(glm::mat4(1.0f), glm::vec3(1, 1, 0.5f));

	// RGB texture (3 channels), 2x1
	unsigned char pixels[] = { 255, 0, 255, 255, 255, 0 };
	unsigned char red_pixels[] = { 255, 0, 0, 255, 0, 0 };
	unsigned char grey_pixels[] = { 135, 135, 135,135, 135, 135 };
	Texture t_magYel = makeTexture(7, 1, 3, pixels, true);
	Texture t_red = makeTexture(7, 1, 3, red_pixels, true);
	Texture t_grey = makeTexture(1, 1, 3, grey_pixels, true);

	Texture wall_normal = loadTexture("../../resources/textures/Blue.jpg");
	Texture wall_diffuse = loadTexture("../../resources/textures/Blue.jpg");
	float ss_gloss = 4.0f;

	Shader standard      = loadShader("../../resources/shaders/mvpmat.vert", "../../resources/shaders/mvpmat.frag");
	Shader mirror_shader = loadShader("../../resources/shaders/mirror.vert", "../../resources/shaders/mirror.frag");
	Shader phong_shader  = loadShader("../../resources/shaders/phong.vert",  "../../resources/shaders/phong.frag");
	Shader passG_shader  = loadShader("../../resources/shaders/pass_g.vert", "../../resources/shaders/pass_g.frag");
	Shader passL_shader  = loadShader("../../resources/shaders/pass_l.vert", "../../resources/shaders/pass_l.frag");
	Shader passC_shader  = loadShader("../../resources/shaders/pass_c.vert", "../../resources/shaders/pass_c.frag");

	Shader * shaderPtr = nullptr;
	shaderPtr = &mirror_shader;

	Framebuffer gBuffer = { 0, 800, 600 };
	Framebuffer lBuffer = { 0, 800, 600 };
	Framebuffer screen = { 0, 800, 600 };

	glm::mat4 *projPtr, *viewPtr;

	float pos_x = 0;

	// Camera
	glm::mat4 cam_view = glm::lookAt(glm::vec3(0, pos_x, -4),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0));

	glm::mat4 cam_proj = glm::perspective(145.f, 800.f / 600.f, .01f, 100.f);


	projPtr = &cam_proj;
	viewPtr = &cam_view;

	// Model
	glm::mat4 go_model(1.0);

	// Light
	float l_x = 1, l_y = -1, l_z = 1;
	glm::vec3 l_dir = glm::normalize(glm::vec3(1, -1, -1));
	glm::mat4 l_proj = glm::ortho<float>(-10, 10, -10, 10, -10, 10);
	glm::mat4 l_view = glm::lookAt(-l_dir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::vec4 l_color = glm::vec4(.7f, .6f, .9f, 1);
	float     l_intensity = 1.f;
	glm::vec4 l_ambient = glm::vec4(.2f, .2f, .01f, 1);
	int       l_type = 0;

	glm::mat4 rotCamMat(1.0);

	float angle = 0, up_angle = 0, wall_angle = 0, cutoff = 0;



	// Simple matrix that converts from clip-space (-1, 1) to UV space (0, 1)
	glm::mat4 clipToUV{ 0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f };

	while (context.step())
	{
		if (context.getKey('A'))
			l_x -= 0.1f;
		else if (context.getKey('D'))
			l_x += 0.1f;
		if (context.getKey('W'))
			l_y -= 0.1f;
		else if (context.getKey('S'))
			l_y += 0.1f;
		if (context.getKey('Q'))
			l_z += 0.1f;
		else if (context.getKey('E'))
			l_z -= 0.1f;
		if (context.getKey('Z'))
			wall_angle += 0.01f;
		else if (context.getKey('X'))
			wall_angle -= 0.01f;

		projPtr = &cam_proj;
		viewPtr = &cam_view;

		cam_view = glm::lookAt(glm::vec3(pos_x, 3, -4),
			glm::vec3(0, 0, 0),
			glm::vec3(0, 1, 0));

		rotCamMat = glm::rotate(angle, glm::vec3(0, 1, 0)) * glm::rotate(up_angle, glm::vec3(1, 0, 0));

		float timeVal = (float)context.getTime();
		glm::mat4 rotMat = glm::rotate(timeVal, glm::vec3(0, 1, 0));



		//////////////////////////////////////////////////
		////  G pass
		clearFrameBuffer(screen);
		setFlags(RenderFlag::DEPTH);
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer.handle);
		glViewport(0, 0, 800, 600);

		// Phong Sphere
		int loc = 0, slot = 0;
		setUniforms(passG_shader, loc, slot,
			//cam_proj, cam_view,
			*projPtr * *viewPtr * rotCamMat * go_model, //proj view model matrix
			*viewPtr * rotCamMat * go_model, // view model matrix
			wall_diffuse,
			12.0f);// , // wall data
		s0_draw(gBuffer, passG_shader, goList[0]);

		// Phong Sphere
		loc = 0, slot = 0;
		setUniforms(passG_shader, loc, slot,
			//cam_proj, cam_view,
			*projPtr * *viewPtr * rotCamMat * go_model, //proj view model matrix
			*viewPtr * rotCamMat * go_model, // view model matrix
			wall_diffuse,
			12.0f);// , // wall data
		s0_draw(gBuffer, passG_shader, goList[1]);


		////////////////////////////////////////////////
		//  L pass
		clearFrameBuffer(screen);
		setFlags(RenderFlag::DEPTH);
		glBindFramebuffer(GL_FRAMEBUFFER, lBuffer.handle);
		glViewport(0, 0, 800, 600);

		glm::mat4 clipToUVLightProjLightViewInverseCamView_Matrix = clipToUV * l_proj * l_view * glm::inverse(cam_view);

		// Phong Sphere
		loc = 0, slot = 0;
		setUniforms(passL_shader, loc, slot,
			//cam_proj, cam_view,
			clipToUVLightProjLightViewInverseCamView_Matrix * go_model, l_color, l_intensity, gBuffer.targets[1], gBuffer.targets[2]);// , // wall data
		s0_draw(lBuffer, passL_shader, goList[0]);

		// Phong Sphere
		loc = 0, slot = 0;
		setUniforms(passL_shader, loc, slot,
			//cam_proj, cam_view,
			clipToUVLightProjLightViewInverseCamView_Matrix * go_model, l_color, l_intensity, gBuffer.targets[1], gBuffer.targets[2]);// , // wall data
		s0_draw(lBuffer, passL_shader, goList[1]);

		/*
		// Mirror
		loc = 0, slot = 0;
		setUniforms(passG_shader, loc, slot,
			//cam_proj, cam_view,
			*projPtr, *viewPtr,
			//go_model, wall_diffuse, wall_specular, wall_normal, wall_gloss, // wall data
			mirror_model * mirrorTraMat * mirrorRotMat, mBuffer.targets[0], wall_normal, cutoff, true);// , // wall data
		s0_draw(screen, passC_shader, goList[1]);

		// Wall
		loc = 0, slot = 0;
		setUniforms(passG_shader, loc, slot,
			//cam_proj, cam_view,
			*projPtr, *viewPtr,
			//go_model, wall_diffuse, wall_specular, wall_normal, wall_gloss, // wall data
			glm::mat4(1.0f), t_magYel, wall_normal, cutoff, false);// , // wall data
		s0_draw(screen, passC_shader, goList[2]);*/
	}
	
	context.term();
	delete[] verts;

	return 0;
}

glm::vec3 getNormal(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2)
{
	glm::vec3 v0(p1 - p0);
	glm::vec3 v1(p2 - p0);

	glm::vec3 retval = glm::cross(v1, v0);

	return retval;
}