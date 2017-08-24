#include "..\include\graphics\draw.h"

void s0_draw(const Framebuffer & f, const Shader & s, const Geometry & g)
{
	glBindFramebuffer(GL_FRAMEBUFFER, f.handle);
	glUseProgram(s.handle);
	glBindVertexArray(g.handle);

	glViewport(0, 0, f.width, f.height);

	glDrawElements(GL_TRIANGLES, g.size, GL_UNSIGNED_INT, 0);
}

void setUniform(const Shader & shader, int location, float value)
{
	glProgramUniform1f(shader.handle, location, value);
}

void __internal::t_setUniform(const Shader &s, int &loc_io, int &tex_io, float val)
{
	glProgramUniform1f(s.handle, loc_io++, val);
}
void __internal::t_setUniform(const Shader &s, int &loc_io, int &tex_io, int val)
{
	glProgramUniform1i(s.handle, loc_io++, val);
}

void __internal::t_setUniform(const Shader &s, int &loc_io, int &tex_io, const Texture &val)
{
	glActiveTexture(GL_TEXTURE0 + tex_io);
	glBindTexture(GL_TEXTURE_2D, val.handle);
	glProgramUniform1i(s.handle, loc_io++, tex_io++);
}

//void setUniform(const Shader & shader, int location, int value)
//{
//	glProgramUniform1i(shader.handle, location, value);
//}
//
//void setUniform(const Shader & shader, int location, const Texture & value, unsigned slot)
//{
//	glActiveTexture(GL_TEXTURE0 + slot);
//	glBindTexture(GL_TEXTURE_2D, value.handle);
//	glProgramUniform1i(shader.handle, location, slot);
//}

void clearFrameBuffer(const Framebuffer & fb)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fb.handle);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
