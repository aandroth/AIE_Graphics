#pragma once

#include "RenderObjects.h"

void s0_draw(const Framebuffer &f, const Shader &s, const Geometry &g);

void clearFrameBuffer(const Framebuffer & fb, bool color = true, bool depth = true);

enum RenderFlag {DEPTH = 1};
void setFlags(int flags);

void setUniform(const Shader &shader, int location, float value);
void setUniform(const Shader &shader, int location, int value);
void setUniform(const Shader &shader, int location, const Texture &value, unsigned slot);

namespace __internal
{
	void t_setUniform(const Shader &s, int &loc_io, int &tex_io, float val);
	void t_setUniform(const Shader &s, int &loc_io, int &tex_io, int val);
	void t_setUniform(const Shader &s, int &loc_io, int &tex_io, const Texture &val);
	void t_setUniform(const Shader &s, int &loc_io, int &tex_io, const glm::vec3 &val);
	void t_setUniform(const Shader &s, int &loc_io, int &tex_io, const glm::vec4 &val);
	void t_setUniform(const Shader &s, int &loc_io, int &tex_io, const glm::mat4 &val);
	void t_setUniform(const Shader &s, int &loc_io, int &tex_io, const Camera &val);
	void t_setUniform(const Shader &s, int &loc_io, int &tex_io, const Model &val);
}

template<typename T, typename ...U>
void setUniforms(const Shader &s, int &loc_io, int &tex_io, const T &val, U && ...uniforms)
{
	__internal::t_setUniform(s, loc_io, tex_io, val);
	setUniforms(s, loc_io, tex_io, uniforms...);
}

template<typename T>
void setUniforms(const Shader &s, int &loc_io, int &tex_io, const T &val)
{
	__internal::t_setUniform(s, loc_io, tex_io, val);
}


void clearFrameBuffer(const Framebuffer & fb, bool color, bool depth);