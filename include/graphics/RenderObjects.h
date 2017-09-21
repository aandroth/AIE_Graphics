#pragma once

#include "glinc.h"
#include "glm\geometric.hpp"
#include "glm\glm.hpp"
#include "glm\ext.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtx\matrix_interpolation.hpp"

struct Vertex;

struct Geometry 
{
	unsigned handle, vbo, ibo, size;
};

void SolveTangents(Vertex *v, size_t vsize,
	const unsigned *indices, size_t isize);

Geometry MakeGeometry(const Vertex *vertices, size_t vsize,
	const unsigned *indices, size_t isize);

void freeGeometry(Geometry &g);

struct Shader
{
	//unsigned handle;
	GLuint handle;
};

Shader makeShader(const char *vsource, const char *fsource);
void freeShader(Shader &s);

struct Texture
{
	unsigned handle;
};
// RGBA = 4 channels
// 512x512 image = 262144 pizels * 4 channels = ~1million
Texture makeTexture(unsigned w, unsigned h, unsigned c, const void *pixels, bool isFloat = false);

void freeTexture(Texture &t);

struct Framebuffer
{
	unsigned handle, width, height, nTargets;
	Texture targets[8];
	Texture depthTarget;
};

Framebuffer makeFrameBuffer(unsigned w, unsigned h, unsigned c, bool hasDepth, unsigned nTargets, unsigned nFloatTargets);

void freeFrameBuffer(Framebuffer &fb);

struct Transform
{
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation; // rotation is one of 3 possibilities-- vec3 of euler angles, quaternion, or a mat3 of rotation directions

	glm::vec3 fwd, rt;

	Transform* parent = nullptr;

	glm::mat4 getLocal() const
	{
		glm::mat4 Local;// = glm::mat4{ glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 0, 1, 0), glm::vec4(0, 0, 0, 1) };

		glm::vec3 up = glm::cross(fwd, rt);

		glm::mat4(glm::vec4(rt, 0), glm::vec4(up, 0), glm::vec4(fwd, 0), glm::vec4(0, 0, 0, 1));
		
		Local = glm::translate(position) * glm::yawPitchRoll(rotation.y, rotation.x, rotation.z) * glm::scale(scale);
		 
		return Local;
	}
	glm::mat4 getGlobal() const
	{
		if (parent == nullptr)
			return getLocal();

		return parent->getGlobal() * getLocal();
	}

	//void axisAngle(float angle, const glm::vec3 &axis = { 0, 1, 0 })
	//{
	//	
	//}
	//void lookAt(const glm::vec3 &target, const glm::vec3 &up = { 0, 1, 0 })
	//{
	//	rotation = target - position;
	//}
};

struct Camera
{
	Transform transform;

	float fov = 45.f, aspect = 8.f / 6.f, near = 0.01f, far = 100.f;

	//glm::mat4 getProjection() const;
	//glm::mat4 getView() const;
};

struct FlyCamera
{
	Camera camera;

	float speed;

	//void update(float dt);
};

struct OrbitCamera
{
	Camera camera;

	float speed;

	glm::vec3 target;

	//void update(float dt);
};

struct Mesh
{
	Transform transform;

	Geometry geometry;
};

glm::vec3 projection(const glm::vec3 &norm, const glm::vec3 &v);

glm::vec3 reflection(const glm::vec3 &norm, const glm::vec3 &v);