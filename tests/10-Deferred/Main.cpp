
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
	Shader phong_shader  = loadShader("../../resources/shaders/phong.vert", "../../resources/shaders/phong.frag");
	Shader passG_shader = loadShader("../../resources/shaders/pass_g.vert", "../../resources/shaders/pass_g.frag");
	Shader passC_shader  = loadShader("../../resources/shaders/pass_c.vert", "../../resources/shaders/pass_c.frag");

	Shader * shaderPtr = nullptr;
	shaderPtr = &mirror_shader;

	Framebuffer screen = { 0, 800, 600 };

	glm::mat4 *projPtr, *viewPtr;

	float pos_x = 0;

	// Camera
	glm::mat4 cam_view = glm::lookAt(glm::vec3(0, pos_x, -4),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0));

	glm::mat4 cam_proj = glm::perspective(145.f, 800.f / 600.f, .01f, 100.f);

	// Mirror
	float mirror_angle = 0, mirror_angle_y = 0, mirror_x = 0, mirror_y = 0, mirror_z = 0;


	// MirrorCam
	glm::mat4 mirror_view = glm::lookAt(glm::vec3(mirror_x + 0.5f, mirror_y - 1.5f, mirror_z - 0.5f),
		glm::vec3(0, 0, 1),
		glm::vec3(0, 1, 0));

	glm::mat4 mirror_proj = glm::perspective(200.f, 1.f, .01f, 100.f);

	projPtr = &cam_proj;
	viewPtr = &cam_view;

	// Model
	glm::mat4 go_model(1.0);

	// Light
	float l_x = 1, l_y = -1, l_z = 1;
	glm::vec3 l_data = glm::normalize(glm::vec3(1, -1, 1));
	glm::vec4 l_color = glm::vec4(.7f, .6f, .9f, 1);
	float     l_intensity = 1.f;
	glm::vec4 l_ambient = glm::vec4(.2f, .2f, .01f, 1);
	int       l_type = 0;

	Framebuffer fBuffer = makeFrameBuffer(800, 600, 4, true, 3, 1);
	Framebuffer mBuffer = makeFrameBuffer(800, 600, 4, true, 1, 0);

	glm::mat4 rotCamMat(1.0);

	float angle = 0, up_angle = 0, wall_angle = 0, cutoff = 0;

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
		l_data = glm::normalize(glm::vec3(l_x, l_y, l_z));
		if (context.getKey('Z'))
			wall_angle += 0.01f;
		else if (context.getKey('X'))
			wall_angle -= 0.01f;

		// Move the mirror around
		if (context.getKey('J'))
			mirror_x -= 0.01f;
		else if (context.getKey('L'))
			mirror_x += 0.01f;
		if (context.getKey('U'))
			mirror_y += 0.01f;
		else if (context.getKey('O'))
			mirror_y -= 0.01f;
		if (context.getKey('I'))
			mirror_z -= 0.01f;
		else if (context.getKey('K'))
			mirror_z += 0.01f;
		if (context.getKey('['))
			mirror_angle -= 0.01f;
		else if (context.getKey(']'))
			mirror_angle += 0.01f;
		if (context.getKey(','))
			mirror_angle_y -= 0.01f;
		else if (context.getKey('.'))
			mirror_angle_y += 0.01f;
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

		cam_view = glm::lookAt(glm::vec3(pos_x, 3, -4),
			glm::vec3(0, 0, 0),
			glm::vec3(0, 1, 0));

		rotCamMat = glm::rotate(angle, glm::vec3(0, 1, 0)) * glm::rotate(up_angle, glm::vec3(1, 0, 0));

		float timeVal = (float)context.getTime();
		glm::mat4 rotMat = glm::rotate(timeVal, glm::vec3(0, 1, 0));



		////////////////////////////////////////////////
		//  Screen pass
		clearFrameBuffer(screen);
		setFlags(RenderFlag::DEPTH);
		glBindFramebuffer(GL_FRAMEBUFFER, screen.handle);
		glViewport(0, 0, 800, 600);

		// Phong Sphere
		int loc = 0, slot = 0;
		setUniforms(passG_shader, loc, slot,
			//cam_proj, cam_view,
			*projPtr * *viewPtr * glm::translate(go_model, glm::vec3(mirror_x, mirror_y, mirror_z)) * rotCamMat * go_model, //proj view model matrix
			*viewPtr * glm::translate(go_model, glm::vec3(mirror_x, mirror_y, mirror_z)) * rotCamMat * go_model, // view model matrix
			wall_diffuse,
			12.0f);// , // wall data
		s0_draw(screen, passG_shader, goList[0]);

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

		// Phong Sphere
		loc = 0, slot = 0;
		setUniforms(passG_shader, loc, slot,
			//cam_proj, cam_view,
			*projPtr * *viewPtr * glm::translate(go_model, glm::vec3(mirror_x, mirror_y, mirror_z)) * rotCamMat * go_model, //proj view model matrix
			*viewPtr * glm::translate(go_model, glm::vec3(mirror_x, mirror_y, mirror_z)) * rotCamMat * go_model, // view model matrix
			wall_diffuse,
			12.0f);// , // wall data
		s0_draw(screen, passG_shader, goList[1]);
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