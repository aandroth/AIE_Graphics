#pragma once
#include "RenderObjects.h"
#include "Vertex.h"
# define M_PI           3.14159265358979323846

struct SpecGloss
{
	Geometry geo;

	glm::mat4 model;
	Texture diffuse;
	Texture specular;
	Texture mormal;
	float gloss;
};

struct StandardLight
{

};

struct DirectionalLight
{
	glm::vec3 target;
	float range;

	glm::vec3 direction;
	//0
	glm::mat4 getProj() const
	{
		return glm::ortho(-range, range, -range, range, -range, range);
	}

	//1
	glm::mat4 getView()
	{
		return glm::lookAt(-direction + target, target, glm::vec3(0, 1, 0));
	}
	
	glm::vec4 color; //2
	float intensity; //3
};

namespace __internal
{
	void t_setUniform(const Shader &s, int &loc);
}

Geometry MakeNGon(int sideCount);