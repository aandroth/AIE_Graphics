#pragma once

#include "RenderObjects.h"

void s0_draw(const Framebuffer &f, const Shader &s, const Geometry &g);

//void setUniform(const Shader &shader, int location, float value);
//void setUniform(const Shader &shader, int location, int value);
//void setUniform(const Shader &shader, int location, const Texture &value, unsigned slot);

namespace __internal
{
	void t_setUniform(const Shader &s, int &loc_io, int &tex_io, float val)
	{
		glProgramUniform1f(s.handle, loc_io++, val);
	}
	void t_setUniform(const Shader &s, int &loc_io, int &tex_io, int val)
	{
		glProgramUniform1i(s.handle, loc_io++, val);
	}

	void t_setUniform(const Shader &s, int &loc_io, int &tex_io, const Texture &val)
	{
		glActiveTexture(GL_TEXTURE0 + tex_io);
		glBindTexture(GL_TEXTURE_2D, val.handle);
		glProgramUniform1i(s.handle, loc_io++, tex_io++);
	}
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


void clearFrameBuffer(const Framebuffer & fb);