#pragma once

#include "glinc.h"
#include "glm\geometric.hpp"
#include "glm\glm.hpp"
#include "glm\gtc\type_ptr.hpp"

struct Vertex;

struct Geometry 
{
	unsigned handle, vbo, ibo, size;
};

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

struct Framebuffer
{
	unsigned handle;
	unsigned width, height;
};

struct Texture
{
	unsigned handle;
};

Texture makeTexture(unsigned w, unsigned h, unsigned c, const unsigned char *pixels);

void freeTexture(Texture &t);

struct Transform
{
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;

	Transform* parent = nullptr;

	glm::mat4 getLocal() const;
	glm::mat4 getGlobal() const;

	void axisAngle(float angle, const glm::vec3 &axis = { 0, 1, 0 });
	void lookAt(const glm::vec3 &target, const glm::vec3 &up = { 0, 1, 0 });
};

struct Camera
{
	Transform transform;

	float fov = 45.f, aspect = 8.f / 6.f, near = 0.01f, far = 100.f;

	glm::mat4 getProjection() const;
	glm::mat4 getView() const;
};

struct FlyCamera
{
	Camera camera;

	float ..                                                                                           
};

struct OrbitCamera
{
	Camera camera;

	float speed;

	glm::vec3 target;

	void update(float dt);
};

struct Mesh
{
	Transform transform;

	Geometry geometry;
};

